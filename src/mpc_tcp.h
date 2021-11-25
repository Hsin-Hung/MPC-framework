#include <sys/poll.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#ifndef MPC_TCP_H
#define MPC_TCP_H

#define PORT 8000
#define TRUE 1
#define FALSE 0

int succ_sock, pred_sock;

struct TCP_Request
{

    int socket_fd;
    struct sockaddr_in socket_addr;
    int count;
    struct pollfd fds[200];
    void *buf;
    int flag; /* send: 0, receive: 1 */
};

int get_socket(int party_rank);

char *get_address(int rank);
// args: the number of parties, assume we have mapping between rank -> ip:port
int TCP_Init(int argc, char **argv);

/* assign a rank for this party */
int TCP_Comm_rank(int *rank);

/* assign the number of parties, which will always be 3 */
int TCP_Comm_size(int *size);

// Performs a standard-mode blocking send.
int TCP_Send(const void *buf, int count, int dest, int data_size);

//  Performs a standard-mode blocking receive.
int TCP_Recv(void *buf, int count, int source, int data_size);

//  Starts a standard-mode, nonblocking send
int TCP_Isend(const void *buf, int count, int dest, int data_size, struct TCP_Request *r);

// Starts a standard-mode, nonblocking receive
int TCP_Irecv(void *buf, int count, int source, int data_size, struct TCP_Request *r);

// Waits for an MPI send or receive to complete.
int TCP_Wait(struct TCP_Request *r);

int TCP_Connect(int dest);

int TCP_Accept(int source);

#endif