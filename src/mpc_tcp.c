#define _XOPEN_SOURCE 600
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>
#include <netinet/tcp.h>
#include <liburing.h>
#include "mpc_tcp.h"
#include "comm.h"
#include "config.h"

int succ_sock;
int pred_sock;

#ifdef UKL
extern void set_bypass_limit(int val);
extern void set_bypass_syscall(int val);
#else
#define set_bypass_limit(X) do {} while(0)
#define set_bypass_syscall(X) do {} while(0)
#endif

#ifndef MAX_CONN_TRIES
#define MAX_CONN_TRIES 8
#endif

#ifdef URING_TCP
#define QUEUE 					10
#define EVENT_TYPE_READ         1
#define EVENT_TYPE_WRITE        2

struct io_uring ring;

struct request {
	int event_type;
	int iovec_count;
	int socket;
	struct iovec iov[];
};
#endif

extern struct secrecy_config config;

#ifdef TCP_TRACE
struct Record
{
	struct timespec start;
	struct timespec end;
	size_t size;
};

#define RECORDS 1000

struct Record *sends;
struct Record *receives;

size_t send_count = 0;
size_t receive_count = 0;
#endif

int get_socket(unsigned int party_rank)
{
    if (party_rank == get_succ())
    {
        return succ_sock;
    }
    else
    {
        return pred_sock;
    }
}

/* get the IP address of given rank */
char *get_address(unsigned int rank)
{
    if (rank < config.num_parties)
    {
        return config.ip_list[rank];
    }
    else
    {
        printf("No such rank!");
        return NULL;
    }

    return NULL;
}

int TCP_Init()
{
    /* init party 0 last */
    if (get_rank() == 0)
    {
        TCP_Connect(get_succ());
        TCP_Accept(get_pred());
    }
    else
    {
        TCP_Accept(get_pred());
        TCP_Connect(get_succ());
    }

#ifdef TRACE_TCP
    sends = malloc(sizeof(struct Record) * RECORDS);
    receives = malloc(sizeof(struct Record) * RECORDS);

    if (!sends || !receives)
    {
        exit(1);
    }

    memset(sends, 0, sizeof(struct Record) * RECORDS);
    memset(receives, 0, sizeof(struct Record) * RECORDS);
#endif

#ifdef URING_TCP
	io_uring_queue_init(QUEUE, &ring, 0);
#endif

    set_bypass_limit(10);

    return 0;
}

void calc_diff(struct timespec *diff, struct timespec *bigger, struct timespec *smaller)
{
        if (smaller->tv_nsec > bigger->tv_nsec)
        {
                diff->tv_nsec = 1000000000 + bigger->tv_nsec - smaller->tv_nsec;
                diff->tv_sec = bigger->tv_sec - 1 - smaller->tv_sec;
        }
        else
        {
                diff->tv_nsec = bigger->tv_nsec - smaller->tv_nsec;
                diff->tv_sec = bigger->tv_sec - smaller->tv_sec;
        }
}

int TCP_Finalize(){
#ifdef TRACE_TCP
    int i;
    struct timespec diff;
    FILE *snds = fopen("/sends.csv", "w");
    FILE *recvs = fopen("/receives.csv", "w");

    printf("Saw %ld sends and %ld receives\n", send_count, receive_count);

    fprintf(snds, "Time,Size\n");
    fprintf(recvs, "Time,Size\n");
    for (i = 0; i < send_count; i++)
    {
        calc_diff(&diff, &sends[i].end, &sends[i].start);
        fprintf(snds, "%ld.%09ld,%ld\n", diff.tv_sec, diff.tv_nsec, sends[i].size);
    }
    for (i = 0; i < receive_count; i++)
    {
        calc_diff(&diff, &receives[i].end, &receives[i].start);
        fprintf(recvs, "%ld.%09ld,%ld\n", diff.tv_sec, diff.tv_nsec, receives[i].size);
    }

#ifdef URING_TCP
	io_uring_queue_exit(ring)
#endif

    fflush(snds);
    fflush(recvs);

    free(sends);
    free(receives);

    fclose(snds);
    fclose(recvs);
#endif

    close(succ_sock);
    close(pred_sock);

}

int TCP_Connect(int dest)
{
    int sock = 0, option = 1;
    struct sockaddr_in serv_addr;
    int tries = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(config.port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, get_address(dest), &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    int result = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &option, sizeof(int));
    if (result)
    {
        perror("Error setting TCP_NODELAY ");
        return -1;
    }

    while (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        if (tries < MAX_CONN_TRIES &&
            (errno == ECONNREFUSED || errno == EINTR || errno == ETIMEDOUT || errno == ENETUNREACH))
        {
            // Try and exponential back-off to wait for the other side to come up
            printf("Couldn't connect to peer, waiting %d seconds before trying again.\n", (int)pow(2, tries));
            sleep((int)pow(2, tries));
            tries++;
        }
        else
        {
            perror("Connection Failed ");
            return -1;
        }
    }

    succ_sock = sock;
    return 0;
}

int TCP_Accept(int source)
{

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int result = setsockopt(server_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(int));
    if (result)
    {
        perror("Error setting TCP_NODELAY ");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(config.port);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    pred_sock = new_socket;
    return 0;
}

static int base_send(const void *buf, int count, int dest, int data_size)
{
    ssize_t n;
    void *p = buf;

    set_bypass_syscall(1);
    count = count * data_size;
    while (count > 0)
    {
#ifdef TRACE_TCP
	    if (send_count < RECORDS)
	    {
            clock_gettime(CLOCK_MONOTONIC, &sends[send_count].start);
	        sends[send_count].size = count;
	    }
#endif

        n = send(get_socket(dest), p, count, 0);
        if (n <= 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR || errno == EIO)
            {
                // In these cases we really should try again
                perror("Error returned from send, retrying");
                continue;
            }
            else
            {
                set_bypass_syscall(0);
                perror("Failed to send");
                return -1;
            }
        }

#ifdef TRACE_TCP
	    if (send_count < RECORDS)
	    {
	        clock_gettime(CLOCK_MONOTONIC, &sends[send_count].end);
	        send_count++;
	    }
#endif

        p += n;
        count -= n;
    }

    set_bypass_syscall(0);
    return 0;
}

int TCP_Setup_Send(const void *buf, int count, int dest, int data_size)
{
    return base_send(buf, count, dest, data_size);
}

int bp_count = 0;

#ifdef UKL_SHORTCUT
extern int shortcut_tcp_sendmsg(int fd, struct iovec *iov);
#endif

#ifdef URING_TCP
void uring_send(int sock, void *data, int len)
{
	struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
	struct io_uring_cqe *cqe;
	struct request *req = malloc(sizeof(struct request) + sizeof(struct iovec));

	req->event_type = EVENT_TYPE_WRITE;
	req->socket = sock;
	req->iovec_count = 1;
	req->iov[0].iov_base = malloc(len);
	req->iov[0].iov_len = len;
	memcpy(req->iov[0].iov_base, data, len);

	io_uring_prep_writev(sqe, req->socket, req->iov, req->iovec_count, 0);
	io_uring_sqe_set_data(sqe, req);
	io_uring_submit(&ring);

	io_uring_wait_cqe(&ring, &cqe);
	io_uring_cqe_seen(&ring, cqe);
}
#endif

int TCP_Send(const void *buf, int count, int dest, int data_size)
{
#ifdef UKL_SHORTCUT
    ssize_t n;
    void *p = buf;
    struct iovec iov;

    set_bypass_syscall(1);
    count = count * data_size;
    while (count > 0)
    {

#ifdef TRACE_TCP
	if (send_count < RECORDS)
	{
        clock_gettime(CLOCK_MONOTONIC, &sends[send_count].start);
	    sends[send_count].size = count;
	}
#endif

        iov.iov_base = (void *)p;
        iov.iov_len  = count;
        n = shortcut_tcp_sendmsg(get_socket(dest), &iov);

#ifdef TRACE_TCP
	if (send_count < RECORDS)
	{
	    clock_gettime(CLOCK_MONOTONIC, &sends[send_count].end);
	    send_count++;
	}
#endif

        if (n <= 0)
            return -1;
        p += n;
        count -= n;
    }

    return 0;
#elif defined URING_TCP
	uring_send(get_socket(dest), buf, data_size);
	return 0;
#else
    return base_send(buf, count, dest, data_size);
#endif //UKL_SHORTCUT
}

static int base_recv(void *buf, int count, int source, int data_size)
{
    ssize_t n;
    void *p = buf;

    set_bypass_syscall(1);
    count = count * data_size;
    while (count > 0)
    {
#ifdef TRACE_TCP
    if (receive_count < RECORDS)
	{
	    clock_gettime(CLOCK_MONOTONIC, &receives[receive_count].start);
	    receives[receive_count].size = count;
	}
#endif
        n = read(get_socket(source), p, count);
        if (n <= 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR || errno == EIO)
            {
                // In these cases we really should try again
                perror("Error returned from read, retrying");
                continue;
            }
            else
            {
                set_bypass_syscall(0);
                perror("Failed to read");
                return -1;
            }
        }
#ifdef TRACE_TCP
        if (receive_count < RECORDS)
        {
            clock_gettime(CLOCK_MONOTONIC, &receives[receive_count].end);
            receive_count++;
        }
#endif
        p += n;
        count -= n;
    }

    set_bypass_syscall(0);
    return 0;
}

int TCP_Setup_Recv(void *buf, int count, int source, int data_size)
{
    return base_recv(buf, count, source, data_size);
}

#ifdef UKL_SHORTCUT
extern int shortcut_tcp_recvmsg(int fd, struct iovec *iov);
#endif

#ifdef URING_TCP
void uring_recv(int sock, void  *buf, int len)
{
	struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
	struct io_uring_cqe *cqe;
	struct request *req = malloc(sizeof(struct request) + sizeof(struct iovec));
	struct request *res;

	req->iovec_count = 1;
	req->iov[0].iov_base = malloc(len);
	req->iov[0].iov_len = len;
	req->event_type = EVENT_TYPE_READ;
	req->socket = sock;
	memset(req->iov[0].iov_base, 0, len);


	io_uring_prep_readv(sqe, req->socket, &req->iov[0], req->iovec_count, 0);
	io_uring_sqe_set_data(sqe, req);
	io_uring_submit(&ring);

	cqe = malloc(sizeof(struct io_uring_cqe) + sizeof(struct iovec));

	if(io_uring_wait_cqe(&ring, &cqe) != 0)
	{
		perror("Recieve completion failed.\n");
		exit(0);
	}

	if (cqe->res < 0) 
	{
		fprintf(stderr, "Async request failed: %s for event: %d\n",
		strerror(-cqe->res), req->event_type);
		exit(1);
	}

	res = (struct request *)io_uring_cqe_get_data(cqe);
	memset(buf, (char *)res->iov[0].iov_base, len);

	io_uring_cqe_seen(&ring, cqe);
}
#endif

int TCP_Recv(void *buf, int count, int source, int data_size)
{
#ifdef UKL_SHORTCUT
    ssize_t n;
    void *p = buf;
    struct iovec iov;

    count = count * data_size;
    while (count > 0)
    {
#ifdef TRACE_TCP
	if (receive_count < RECORDS)
	{
	    clock_gettime(CLOCK_MONOTONIC, &receives[receive_count].start);
	    receives[receive_count].size = count;
	}
#endif
        iov.iov_base = (void *)p;
        iov.iov_len  = count;
        n = shortcut_tcp_recvmsg(get_socket(source), &iov);
#ifdef TRACE_TCP
	if (receive_count < RECORDS)
	{
	    clock_gettime(CLOCK_MONOTONIC, &receives[receive_count].end);
	    receive_count++;
	}
#endif

        if (n <= 0)
            return -1;
        p += n;
        count -= n;
    }

    return 0;
#elif defined URING_TCP
	uring_recv(get_socket(source), buf, data_size);
	return 0;
#else
    return base_recv(buf, count, source, data_size);
#endif //UKL_SHORTCUT
}
