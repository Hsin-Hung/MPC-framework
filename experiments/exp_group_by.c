#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

#include "exp-utils.h"

#define COLS 1

/**
 * Evaluates the performance of the batched group-by-count operator.
 **/

int main(int argc, char** argv) {

  if (argc < 2) {
    printf("\n\nUsage: %s <NUM_ROWS>\n\n", argv[0]);
    return -1;
  }

  // initialize communication
  init(argc, argv);

  const long ROWS = atol(argv[argc - 1]); // input size

  const int rank = get_rank();
  const int pred = get_pred();
  const int succ = get_succ();

  // The input tables per party
  BShareTable t1 = {-1, rank, ROWS, 2*COLS, 1};
  allocate_bool_shares_table(&t1);

  if (rank == 0) { //P1
    // Initialize input data and shares
    Table r1;
    generate_random_table(&r1, ROWS, COLS);

    // t1 Bshare tables for P2, P3 (local to P1)
    BShareTable t12 = {-1, 1, ROWS, 2*COLS, 1};
    allocate_bool_shares_table(&t12);
    BShareTable t13 = {-1, 2, ROWS, 2*COLS, 1};
    allocate_bool_shares_table(&t13);

    init_sharing();

    // Generate boolean shares for r1
    generate_bool_share_tables(&r1, &t1, &t12, &t13);

    //Send shares to P2
    TCP_Send(&(t12.contents[0][0]), ROWS*2*COLS, 1, sizeof(BShare));

    //Send shares to P3
    TCP_Send(&(t13.contents[0][0]), ROWS*2*COLS, 2, sizeof(BShare));

    // free temp tables
    free(r1.contents);
    free(t12.contents);
    free(t13.contents);
  }
  else if (rank == 1) { //P2
    TCP_Recv(&(t1.contents[0][0]), ROWS*2*COLS, 0, sizeof(BShare));
  }
  else { //P3
    TCP_Recv(&(t1.contents[0][0]), ROWS*2*COLS, 0, sizeof(BShare));
  }

  //exchange seeds
  exchange_rsz_seeds(succ, pred);

  struct timeval begin, end;
  long seconds, micro;
  double elapsed;

  AShare *counters = malloc(ROWS*sizeof(AShare));
  AShare *remote_counters = malloc(ROWS*sizeof(AShare));

  BShare *rand_a = malloc(2*(ROWS-1)*sizeof(BShare));
  BShare *rand_b = malloc(2*(ROWS-1)*sizeof(BShare));

  // initialize counters
  for (int i=0; i<ROWS; i++) {
    counters[i] = rank % 2;
    remote_counters[i] = succ % 2;
  }

  // initialize rand bits (all equal to 1)
  for (int i=0; i<2*(ROWS-1); i++) {
    rand_a[i] = (unsigned int) 1;
    rand_b[i] = (unsigned int) 1;
  }

  /* =======================================================
     1. Measure group-by
  ======================================================== */
  // start timer
  gettimeofday(&begin, 0);

  unsigned key_indices[1] = {0};
  group_by_count_micro(&t1, key_indices, 1, counters, remote_counters, rand_b,
                                                                       rand_a);

  // stop timer
  gettimeofday(&end, 0);
  seconds = end.tv_sec - begin.tv_sec;
  micro = end.tv_usec - begin.tv_usec;
  elapsed = seconds + micro*1e-6;

  if (rank == 0) {
    printf("%ld\tGROUP-BY\t%.3f\n", ROWS, elapsed);
  }

  free(t1.contents); free(counters); free(remote_counters);
  free(rand_a); free(rand_b);

  // tear down communication
  TCP_Finalize();
  return 0;
}
