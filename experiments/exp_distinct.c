#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

#include "exp-utils.h"

#define SHARE_TAG 193
#define PRIVATE static
#define COLS 1

/**
 * Evaluates the performance of the distinct operator.
 **/

int main(int argc, char** argv) {

  if (argc < 2) {
    printf("\n\nUsage: %s <NUM_ROWS>\n\n", argv[0]);
    return -1;
  }

  // initialize communication
  init(argc, argv);

  const long ROWS = atol(argv[1]); // input size

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
    MPI_Send(&(t12.contents[0][0]), ROWS*2*COLS, MPI_LONG_LONG, 1, SHARE_TAG, MPI_COMM_WORLD);

    //Send shares to P3
    MPI_Send(&(t13.contents[0][0]), ROWS*2*COLS, MPI_LONG_LONG, 2, SHARE_TAG, MPI_COMM_WORLD);

    // free temp tables
    free(r1.contents);
    free(t12.contents);
    free(t13.contents);

  }
  else if (rank == 1) { //P2
    MPI_Recv(&(t1.contents[0][0]), ROWS*2*COLS, MPI_LONG_LONG, 0, SHARE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  else { //P3
    MPI_Recv(&(t1.contents[0][0]), ROWS*2*COLS, MPI_LONG_LONG, 0, SHARE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  //exchange seeds
  exchange_rsz_seeds(succ, pred);
   
  struct timeval begin, end;
  long seconds, micro;
  double elapsed;
  BitShare *res = malloc(ROWS*sizeof(BitShare));

  /* =======================================================
     1. Measure distinct batched
  ======================================================== */
  // start timer
  gettimeofday(&begin, 0);

  distinct_batch(&t1, 0, res, ROWS-1);
  
  // stop timer
  gettimeofday(&end, 0);
  seconds = end.tv_sec - begin.tv_sec;
  micro = end.tv_usec - begin.tv_usec;
  elapsed = seconds + micro*1e-6;

  if (rank == 0) {
    printf("%d\tDISTINCT-BATCH\t%ld\t%.3f\n", COLS, ROWS, elapsed);
  }

  free(res); free(t1.contents);

  // tear down communication
  MPI_Finalize();
  return 0;
}