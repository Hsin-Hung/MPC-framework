#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

#include "exp-utils.h"

#define SHARE_TAG 193
#define COLS 1

/**
 * Evaluates the performance of the batched join operator.
 **/

int main(int argc, char** argv) {

  if (argc < 3) {
    printf("\n\nUsage: %s <NUM_ROWS_1> <NUM_ROWS_2>\n\n", argv[0]);
    return -1;
  }

  // initialize communication
  init(argc, argv);

  const long ROWS1 = atol(argv[1]); // input1 size
  const long ROWS2 = atol(argv[2]); // input2 size

  const int rank = get_rank();
  const int pred = get_pred();
  const int succ = get_succ();

  // The input tables per party
  BShareTable t1 = {-1, rank, ROWS1, 2*COLS, 1};
  allocate_bool_shares_table(&t1);
  BShareTable t2 = {-1, rank, ROWS2, 2*COLS, 2};
  allocate_bool_shares_table(&t2);

  // if (rank == 0) { //P1
  //   // Initialize input data and shares
  //   Table r1, r2;
  //   generate_random_table(&r1, ROWS1, COLS);
  //   generate_random_table(&r2, ROWS2, COLS);

  //   // t1 Bshare tables for P2, P3 (local to P1)
  //   BShareTable t12 = {-1, 1, ROWS1, 2*COLS, 1};
  //   allocate_bool_shares_table(&t12);
  //   BShareTable t13 = {-1, 2, ROWS1, 2*COLS, 1};
  //   allocate_bool_shares_table(&t13);

  //   // t2 Bshare tables for P2, P3 (local to P1)
  //   BShareTable t22 = {-1, 1, ROWS2, 2*COLS, 2};
  //   allocate_bool_shares_table(&t22);
  //   BShareTable t23 = {-1, 2, ROWS2, 2*COLS, 2};
  //   allocate_bool_shares_table(&t23);
    
  //   init_sharing();

  //   // Generate boolean shares for r1
  //   generate_bool_share_tables(&r1, &t1, &t12, &t13);
  //   // Generate boolean shares for r2
  //   generate_bool_share_tables(&r2, &t2, &t22, &t23);

  //   //Send shares to P2
  //   MPI_Send(&(t12.contents[0][0]), ROWS1*2*COLS, MPI_LONG_LONG, 1, SHARE_TAG, MPI_COMM_WORLD);
  //   MPI_Send(&(t22.contents[0][0]), ROWS2*2*COLS, MPI_LONG_LONG, 1, SHARE_TAG, MPI_COMM_WORLD);

  //   //Send shares to P3
  //   MPI_Send(&(t13.contents[0][0]), ROWS1*2*COLS, MPI_LONG_LONG, 2, SHARE_TAG, MPI_COMM_WORLD);
  //   MPI_Send(&(t23.contents[0][0]), ROWS2*2*COLS, MPI_LONG_LONG, 2, SHARE_TAG, MPI_COMM_WORLD);

  //   // free temp tables
  //   free(r1.contents);
  //   free(t12.contents);
  //   free(t13.contents);
  //   free(r2.contents);
  //   free(t22.contents);
  //   free(t23.contents);

  // }
  // else if (rank == 1) { //P2
  //   MPI_Recv(&(t1.contents[0][0]), ROWS1*2*COLS, MPI_LONG_LONG, 0, SHARE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  //   MPI_Recv(&(t2.contents[0][0]), ROWS2*2*COLS, MPI_LONG_LONG, 0, SHARE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  // }
  // else { //P3
  //   MPI_Recv(&(t1.contents[0][0]), ROWS1*2*COLS, MPI_LONG_LONG, 0, SHARE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  //   MPI_Recv(&(t2.contents[0][0]), ROWS2*2*COLS, MPI_LONG_LONG, 0, SHARE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  // }

  //exchange seeds
  exchange_rsz_seeds(succ, pred);
   
  struct timeval begin, end;
  long seconds, micro;
  double elapsed;

  BShare *res = malloc(ROWS1*ROWS2*sizeof(BShare));
  BShare *remote = malloc(ROWS1*ROWS2*sizeof(BShare));
  Predicate_B p = {EQ, NULL, NULL, 0, 0};

  /* =======================================================
     1. Measure join batched
  ======================================================== */
  // start timer
  gettimeofday(&begin, 0);

  join_b_batch(&t1, &t2, 0, ROWS1, 0, ROWS2, p, remote, res);
  
  // stop timer
  gettimeofday(&end, 0);
  seconds = end.tv_sec - begin.tv_sec;
  micro = end.tv_usec - begin.tv_usec;
  elapsed = seconds + micro*1e-6;

  if (rank == 0) {
    printf("%d\tJOIN-BATCH\t%ld\t%.3f\n", COLS, ROWS1, elapsed);
  }

  free(res); free(t1.contents); free(t2.contents); free(remote);

  // tear down communication
  // MPI_Finalize();
  return 0;
}
