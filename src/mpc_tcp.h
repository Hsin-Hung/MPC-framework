#include <sys/poll.h>

struct TCP_Request
{

    int socket_fd;
    int count;
    struct pollfd fds[200];
    void *buf;
    int flag; /* send: 0, receive: 1 */
};

char *get_address(int rank);
// args: the number of parties, assume we have mapping between rank -> ip:port 
int TCP_Init(int *argc, char ***argv);

/* assign a rank for this party */
int TCP_Comm_rank(int *rank);

/* assign the number of parties, which will always be 3 */
int TCP_Comm_size(int *size);

// Performs a standard-mode blocking send.
int TCP_Send(const void *buf, int count, int dest, int tag);

//  Performs a standard-mode blocking receive.
int TCP_Recv(void *buf, int count, int source, int tag);

//  Starts a standard-mode, nonblocking send
int TCP_Isend(const void *buf, int count, int dest, int tag, struct TCP_Request *r);

// Starts a standard-mode, nonblocking receive
int TCP_Irecv(void *buf, int count, int source, int tag, struct TCP_Request *r);

// Waits for an MPI send or receive to complete.
int TCP_Wait(struct TCP_Request *r);