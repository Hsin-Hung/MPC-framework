#include "comm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SA struct sockaddr
#define PORT 8080

#define PRIVATE static

#define NUM_PARTIES 3
#define XCHANGE_MSG_TAG 7
#define OPEN_MSG_TAG 777

int rank, num_parties;
int initialized = 0;

PRIVATE void check_init(const char *f);

// initialize MPI, rank, num_parties
void init(int argc, char** argv)
{
  TCP_Init(&argc, &argv);
  TCP_Comm_rank(&rank);
  TCP_Comm_size(&num_parties);
  // this protocol works with 3 parties only
  if (rank == 0 && num_parties != NUM_PARTIES) {
     fprintf(stderr, "ERROR: The number of MPI processes must be %d for %s\n", NUM_PARTIES, argv[0]);

   }
}

// finalize MPI: (VK: This doesn't work but I don't know why)
/*void close() {
   MPI_Finalize();
}*/

// exchange boolean shares: this is blocking
BShare exchange_shares(BShare s1)
{
  BShare s2;
  // send s1 to predecessor
  TCP_Send(&s1, 1, get_pred(), XCHANGE_MSG_TAG);
  // // receive remote seed from successor

  TCP_Recv(&s2, 1, get_succ(), XCHANGE_MSG_TAG);
  return s2;
}

// exchange boolean shares: this is blocking
BShare exchange_shares_async(BShare s1)
{
  BShare s2;
  // MPI_Request r1, r2;
  // // receive remote share from successor

  TCP_Irecv(shares2, length, get_succ(), XCHANGE_MSG_TAG, &r2);

  // // send s1 to predecessor
  // MPI_Isend(&s1, 1, MPI_LONG_LONG, get_pred(), XCHANGE_MSG_TAG,
  //                                           MPI_COMM_WORLD, &r1);

  // MPI_Wait(&r1, MPI_STATUS_IGNORE);
  // MPI_Wait(&r2, MPI_STATUS_IGNORE);

  return s2;
}

// exchange boolean shares: this is blocking
unsigned long long exchange_shares_u(unsigned long long s1)
{
  unsigned long long s2;
  // send s1 to predecessor

  TCP_Send(&s1, 1, get_pred(), XCHANGE_MSG_TAG);
  // // receive remote seed from successor

  TCP_Recv(&s2, 1, get_succ(), XCHANGE_MSG_TAG);
  return s2;
}

// Exchanges single-bit boolean shares: this is blocking
BitShare exchange_bit_shares(BitShare s1)
{
  BitShare s2;
  // send s1 to predecessor
  TCP_Send(&s1, 1, get_pred(), XCHANGE_MSG_TAG);
  // // receive remote seed from successor
  
  TCP_Recv(&s2, 1, get_succ(), XCHANGE_MSG_TAG);
  return s2;
}

void exchange_shares_array(const BShare *shares1, BShare *shares2, long length)
{
  // MPI_Request r1, r2;
  // // receive remote share from successor

  TCP_Irecv(shares2, length, get_succ(), XCHANGE_MSG_TAG, &r2);
  // // send s1 to predecessor
  // MPI_Isend(shares1, length, MPI_LONG_LONG, get_pred(),
  //           XCHANGE_MSG_TAG, MPI_COMM_WORLD, &r1);
  // MPI_Wait(&r1, MPI_STATUS_IGNORE);
  // MPI_Wait(&r2, MPI_STATUS_IGNORE);
}

void exchange_shares_array_u(const unsigned long long *shares1,
                             unsigned long long *shares2, int length)
{
  // MPI_Request r1, r2;
  // // receive remote share from successor

    TCP_Irecv(shares2, length, get_succ(), XCHANGE_MSG_TAG, &r2);

  // // send s1 to predecessor
  // MPI_Isend(shares1, length, MPI_UNSIGNED_LONG_LONG, get_pred(),
  //           XCHANGE_MSG_TAG, MPI_COMM_WORLD, &r1);
  // MPI_Wait(&r1, MPI_STATUS_IGNORE);
  // MPI_Wait(&r2, MPI_STATUS_IGNORE);
}

void exchange_a_shares_array(const AShare *shares1, AShare *shares2, int length)
{
  // MPI_Request r1, r2;
  // // receive remote share from successor

    TCP_Irecv(shares2, length, get_succ(), XCHANGE_MSG_TAG, &r2);

  // // send s1 to predecessor
  // MPI_Isend(shares1, length, MPI_LONG_LONG, get_pred(),
  //           XCHANGE_MSG_TAG, MPI_COMM_WORLD, &r1);
  // MPI_Wait(&r1, MPI_STATUS_IGNORE);
  // MPI_Wait(&r2, MPI_STATUS_IGNORE);
}

void exchange_bit_shares_array(const BitShare *shares1, BitShare *shares2,
                               int length)
{
  // MPI_Request r1, r2;
  // // receive remote share from successor

    TCP_Irecv(shares2, length, get_succ(), XCHANGE_MSG_TAG, &r2);
  // // send s1 to predecessor
  // MPI_Isend(shares1, length, MPI_C_BOOL, get_pred(),
  //           XCHANGE_MSG_TAG, MPI_COMM_WORLD, &r1);
  // MPI_Wait(&r1, MPI_STATUS_IGNORE);
  // MPI_Wait(&r2, MPI_STATUS_IGNORE);
}

int get_rank()
{
  check_init(__func__);
  return rank;
}

int get_succ()
{
  check_init(__func__);
  return (get_rank() + 1) % NUM_PARTIES;
}

int get_pred()
{
  check_init(__func__);
  return ((get_rank() + NUM_PARTIES) - 1) % NUM_PARTIES;
}

// [boolean share] open a value in P1 (rank=0)
Data open_b(BShare s)
{
  // P2, P3 send their shares to P1
  if (rank == 1 || rank == 2)
  {
    // MPI_Send(&s, 1, MPI_LONG_LONG, 0, OPEN_MSG_TAG, MPI_COMM_WORLD);
    TCP_Send(&s, 1, 0, OPEN_MSG_TAG);
    return s;
  }
  else if (rank == 0)
  {
    Data msg, res = s;
    // P1 receives shares from P2, P3
    // MPI_Recv(&msg, 1, MPI_LONG_LONG, 1, OPEN_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    TCP_Recv(&msg, 1, 1, OPEN_MSG_TAG);
    res ^= msg;
    // MPI_Recv(&msg, 1, MPI_LONG_LONG, 2, OPEN_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    TCP_Recv(&msg, 1, 2, OPEN_MSG_TAG);
    res ^= msg;
    
    return res;
  }
  else
  {
    fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
    return 1;
  }
}

// [boolean share] open a value in P1 (rank=0)
Data open_bit(BitShare s)
{
  // P2, P3 send their shares to P1
  if (rank == 1 || rank == 2) {
  //     MPI_Send(&s, 1, MPI_C_BOOL, 0, OPEN_MSG_TAG, MPI_COMM_WORLD);
  TCP_Send(&s, 1, 0, OPEN_MSG_TAG);
  return s;
  }
  else if (rank == 0) {
  bool msg, res = s;
  //   // P1 receives shares from P2, P3

    TCP_Recv(&msg, 1, 1, OPEN_MSG_TAG);
    res ^= msg;
    TCP_Recv(&msg, 1, 2, OPEN_MSG_TAG);
    res ^= msg;
  return res;
  }
  else {
  fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
  return 1;
  }
}

// [boolean share] open an array of values in P1 (rank=0)
// MUST BE CALLED ONLY ONCE AS IT MUTATES THE GIVEN TABLE s
void open_b_array(BShare *s, int len, Data res[])
{
  // P2, P3 send their shares to P1
  if (rank == 1 || rank == 2) {
    TCP_Send(s, len, 0, OPEN_MSG_TAG);
  }
  else if (rank == 0) {
    BShare *msg = malloc(len*sizeof(BShare));
    assert(msg!=NULL);
  //   // P1 receives shares from P2, P3
    TCP_Recv(msg, len, 1, OPEN_MSG_TAG);

    for (int i=0; i<len; i++) {
      res[i] = s[i] ^ msg[i];
    }

    TCP_Recv(msg, len, 2, OPEN_MSG_TAG);

    for (int i=0; i<len; i++) {
      res[i] = res[i] ^ msg[i];
    }
    free(msg);
  }
  else {
    fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
  }
}

// [boolean share] open an array of values in P1 (rank=0)
// MUST BE CALLED ONLY ONCE AS IT MUTATES THE GIVEN TABLE s
void open_byte_array(char *s, int len, char res[])
{
  P2, P3 send their shares to P1
  if (rank == 1 || rank == 2) {

    TCP_Send(s, len, 0, OPEN_MSG_TAG);

  }
  else if (rank == 0) {
    char *msg = malloc(len*sizeof(char));
    assert(msg!=NULL);
  // P1 receives shares from P2, P3
    TCP_Recv(msg, len, 1, OPEN_MSG_TAG);

    for (int i=0; i<len; i++) {
      res[i] = s[i] ^ msg[i];
    }

    TCP_Recv(msg, len, 2, OPEN_MSG_TAG);

    for (int i=0; i<len; i++) {
      res[i] = res[i] ^ msg[i];
    }
    free(msg);
  }
  else {
    fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
  }
}

// [arithmetic share] open a value in P1 (rank=0)
Data open_a(AShare s)
{
  // P2, P3 send their shares to P1
  if (rank == 1 || rank == 2) {
  TCP_Send(&s, 1, 0, OPEN_MSG_TAG);

  return s;
  }
  else if (rank == 0) {
    Data msg, res = s;
    // P1 receives shares from P2, P3
    TCP_Recv(&msg, 1, 1, OPEN_MSG_TAG);

    res += msg;
    TCP_Recv(&msg, 1, 2, OPEN_MSG_TAG);

    res += msg;
    return res;
  }
  else {
    fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
    return 1;
  }
}

// [arithmetic share] open an array of arithmetic shares in P1 (rank=0)
void open_array(AShare *s, int len, Data res[])
{

  // P2, P3 send their shares to P1
  if (rank == 1 || rank == 2) {
    TCP_Send(s, len, 0, OPEN_MSG_TAG);
  }
  else if (rank == 0) {
    AShare *msg = malloc(len*sizeof(AShare));
    assert(msg!=NULL);
    // P1 receives shares from P2, P3
    TCP_Recv(msg, len, 1, OPEN_MSG_TAG);

    for (int i=0; i<len; i++) {
      res[i] = s[i] + msg[i];
    }

    TCP_Recv(msg, len, 2, OPEN_MSG_TAG);

    for (int i=0; i<len; i++) {
      res[i] = res[i] + msg[i];
    }
    free(msg);
  }
  else {
    fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
  }
}

// [arithmetic share] open an array of arithmetic shares in P1 (rank=0)
void open_mixed_array(BShare *s, int rows, int cols, Data res[],
                      unsigned *a, int al, unsigned *b, int bl)
{

  assert((al + bl) == cols);
  int len = rows * cols;
  // P2, P3 send their shares to P1
  if (rank == 1 || rank == 2) {
    TCP_Send(s, len, 0, OPEN_MSG_TAG);
  }
  else if (rank == 0) {
    BShare *msg = malloc(rows*cols*sizeof(BShare)); // BShare and AShare have equal size
    assert(msg!=NULL);
    // P1 receives shares from P2, P3
    TCP_Recv(msg, len, 1, OPEN_MSG_TAG);

    for (int i=0; i<len; i+=cols) {
        // If aritmetic
        for (int j=0; j<al; j++) {
          res[i+a[j]] = s[i+a[j]] + msg[i+a[j]];
        }
        // If boolean
        for (int j=0; j<bl; j++) {
          res[i+b[j]] = s[i+b[j]] ^ msg[i+b[j]];
        }
    }

    TCP_Recv(msg, len, 2, OPEN_MSG_TAG);

    for (int i=0; i<len; i+=cols) {
      // If aritmetic
      for (int j=0; j<al; j++) {
        res[i+a[j]] = res[i+a[j]] + msg[i+a[j]];
      }
      // If boolean
      for (int j=0; j<bl; j++) {
        res[i+b[j]] = res[i+b[j]] ^ msg[i+b[j]];
      }
    }
    free(msg);
  }
  else {
    fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
  }
}

void open_bit_array(BitShare *s, int len, bool res[])
{
  //P2, P3 send their shares to P1
  if (rank == 1 || rank == 2) {
      TCP_Send(s, len, 0, OPEN_MSG_TAG);
  }
  else if (rank == 0) {
    BitShare *msg = malloc(len*sizeof(BitShare));
    assert(msg!=NULL);

    // P1 receives shares from P2, P3
    TCP_Recv(msg, len, 1, OPEN_MSG_TAG);
    for (int i=0; i<len; i++) {
      res[i] = s[i] ^ msg[i];
    }

    TCP_Recv(msg, len, 2, OPEN_MSG_TAG);

    for (int i=0; i<len; i++) {
      res[i] = res[i] ^ msg[i];
    }
    free(msg);
  }
  else {
    fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
  }
}

// [boolean share] open a value in all parties
Data reveal_b(BShare s)
{
  Data res = s;

  // P2, P3 send their shares to P1 and receive the result
  if (rank == 1 || rank == 2) {
    TCP_Send(&s, 1, 0, OPEN_MSG_TAG);
    TCP_Recv(&res, 1, 0, OPEN_MSG_TAG);
  }
  else if (rank == 0) {
    Data msg;
    // P1 receives shares from P2, P3
    TCP_Recv(&msg, 1, 1, OPEN_MSG_TAG);
    res ^= msg;
    TCP_Recv(&msg, 1, 2, OPEN_MSG_TAG);
    res ^= msg;

    // P1 sends result to P2, P3
    TCP_Send(&res, 1, 1, OPEN_MSG_TAG);
    TCP_Send(&res, 1, 2, OPEN_MSG_TAG);

  }
  else {
    fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
    return 1;
  }
  return res;
}

// [boolean share] open an array of values in all parties
void reveal_b_array(BShare *s, int len)
{
  // P2, P3 send their shares to P1 and receive the result
  if (rank == 1 || rank == 2) {
    TCP_Send(s, len, 0, OPEN_MSG_TAG);
    TCP_Recv(s, len, 0, OPEN_MSG_TAG);
  }
  else if (rank == 0) {
    Data *msg = malloc(len*sizeof(Data));
    assert(msg!=NULL);
    // P1 receives shares from P2, P3
    TCP_Recv(&msg[0], len, 1, OPEN_MSG_TAG);
    for (int i=0; i<len; i++) {
      s[i]^=msg[i];
    }
    TCP_Recv(&msg[0], len, 2, OPEN_MSG_TAG);
    for (int i=0; i<len; i++) {
      s[i]^=msg[i];
    }
    // P1 sends result to P2, P3
    TCP_Send(s, len, 1, OPEN_MSG_TAG);
    TCP_Send(s, len, 2, OPEN_MSG_TAG);
    free(msg);
  }
  else {
    fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
  }
}

void reveal_b_array_async(BShare *s, int len)
{
  // MPI_Request r1, r2;
  // // P2, P3 send their shares to P1 and receive the result
  // if (rank == 1 || rank == 2) {
  //   MPI_Isend(s, len, MPI_LONG_LONG, 0, OPEN_MSG_TAG, MPI_COMM_WORLD, &r1);
  TCP_Irecv(s, len, 0, OPEN_MSG_TAG, &r2);

  //   MPI_Wait(&r1, MPI_STATUS_IGNORE);
  //   MPI_Wait(&r2, MPI_STATUS_IGNORE);
  // }
  // else if (rank == 0) {
  //   Data *msg = malloc(len*sizeof(Data));
  //   assert(msg!=NULL);
  //   Data *msg2 = malloc(len*sizeof(Data));
  //   assert(msg2!=NULL);
  //   // P1 receives shares from P2, P3
  TCP_Irecv(&msg[0], len, 1, OPEN_MSG_TAG, &r1);
  TCP_Irecv(&msg2[0], len, 2, OPEN_MSG_TAG, &r2);
  //   MPI_Wait(&r1, MPI_STATUS_IGNORE);
  //   MPI_Wait(&r2, MPI_STATUS_IGNORE);

  //   for (int i=0; i<len; i++) {
  //     s[i]^=msg[i];
  //     s[i]^=msg2[i];
  //   }

  //   // P1 sends result to P2, P3
  //   MPI_Isend(s, len, MPI_LONG_LONG, 1, OPEN_MSG_TAG, MPI_COMM_WORLD, &r1);
  //   MPI_Isend(s, len, MPI_LONG_LONG, 2, OPEN_MSG_TAG, MPI_COMM_WORLD, &r2);
  //   MPI_Wait(&r1, MPI_STATUS_IGNORE);
  //   MPI_Wait(&r2, MPI_STATUS_IGNORE);
  //   free(msg); free(msg2);
  // }
  // else {
  //   fprintf(stderr, "ERROR: Invalid rank %d.\n", rank);
  // }
}

// check if MPI has been initialized
PRIVATE void check_init(const char *f)
{
  if (!initialized) {
      fprintf(stderr, "ERROR: init() must be called before %s\n", f);
  }
}
