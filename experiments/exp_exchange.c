#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

#include "exp-utils.h"

/**
 * Evaluates the performance of exchange_array.
 **/

int main(int argc, char** argv) {

  if (argc < 2) {
    printf("\n\nUsage: %s [INPUT_SIZE]\n\n", argv[0]);
    return -1;
  }

  // initialize communication
  init(argc, argv);

  const long ROWS = atol(argv[argc - 1]); // input size

  const int rank = get_rank();
  const int pred = get_pred();
  const int succ = get_succ();
  
  BShare *r1s1, *r1s2;
  r1s1 = malloc(ROWS*sizeof(BShare));
  r1s2 = malloc(ROWS*sizeof(BShare));

  generate_and_share_random_data(rank, r1s1, r1s2, ROWS);

  //exchange seeds
  exchange_rsz_seeds(succ, pred);
   
  struct timeval begin, end;
  long seconds, micro;
  double elapsed;

  /* =======================================================
     1. Measure exchange_array
  ======================================================== */
  // start timer
  gettimeofday(&begin, 0);

  exchange_shares_array(r1s1, r1s2, ROWS);
  
  // stop timer
  gettimeofday(&end, 0);
  seconds = end.tv_sec - begin.tv_sec;
  micro = end.tv_usec - begin.tv_usec;
  elapsed = seconds + micro*1e-6;

  if (rank == 0) {
    printf("BATCHED\t%ld\t%.3f\n", ROWS, elapsed);
  }

  /* =======================================================
     2. Measure synchronous element-wise exchange
  ======================================================== */
  // start timer
  gettimeofday(&begin, 0);

  for (long i=0; i<ROWS; i++) {
    r1s2[i] = exchange_shares(r1s1[i]);
  }
  
  // stop timer
  gettimeofday(&end, 0);
  seconds = end.tv_sec - begin.tv_sec;
  micro = end.tv_usec - begin.tv_usec;
  elapsed = seconds + micro*1e-6;

  if (rank == 0) {
    printf("SYNC\t%ld\t%.3f\n", ROWS, elapsed);
  }

  /* =======================================================
     3. Measure asynchronous element-wise exchange
  ======================================================== */
  // start timer
  gettimeofday(&begin, 0);

  for (long i=0; i<ROWS; i++) {
    r1s2[i] = exchange_shares_async(r1s1[i]);
  }
  
  // stop timer
  gettimeofday(&end, 0);
  seconds = end.tv_sec - begin.tv_sec;
  micro = end.tv_usec - begin.tv_usec;
  elapsed = seconds + micro*1e-6;

  if (rank == 0) {
    printf("ASYNC\t%ld\t%.3f\n", ROWS, elapsed);
  }

  free(r1s1); free(r1s2);

  // tear down communication
  // MPI_Finalize();
  return 0;
}
