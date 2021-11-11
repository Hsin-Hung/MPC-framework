


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

// int MPI_Wait(MPI_Request *request, MPI_Status *status);