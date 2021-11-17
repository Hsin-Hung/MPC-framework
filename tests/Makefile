CC= gcc -std=c99
CFLAGS= -O3 -Wall -lsodium
# MPI= mpicc
SRC=../src
PRIMITIVES= $(SRC)/comm.c $(SRC)/party.c $(SRC)/primitives.c $(SRC)/sharing.c $(SRC)/utils.c $(SRC)/mpc_tcp.c
RELATIONAL= $(SRC)/relational.c
BASELINE= $(SRC)/baseline.c

tests:
		./tests.sh

all: test-conversion test-primitives test-rsz test-share test-select test-rca test-and-all test-equality test-relational test-group test-distinct test-sort test-inequality test-join test-in test-mask test-arithmetic-to-binary test-ageq test-group-join test-group-min-max test-q1 test-q2 test-q3 test-tpch-q4 test-tpch-q4-baseline test-tpch-q13 test-tpch-q13-baseline test-qpwd test-qpwd-baseline test-qcredit test-qcredit-baseline

test-init: test_init.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-init test_init.c $(PRIMITIVES)

test-conn: test_conn.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-conn test_conn.c $(PRIMITIVES)

test-primitives: test_primitives.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-primitives test_primitives.c $(PRIMITIVES)

test-share: test_sharing.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-share test_sharing.c $(PRIMITIVES)

test-query: test_simple_query.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-query test_simple_query.c $(PRIMITIVES)

test-rsz: 	test_rsz.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-rsz test_rsz.c $(PRIMITIVES)

test-and-all:	test_and_all.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-and-all test_and_all.c $(PRIMITIVES)

test-equality:	test_binary_equality.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-equality test_binary_equality.c $(PRIMITIVES)

test-inequality:	test_binary_inequality.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-inequality test_binary_inequality.c $(PRIMITIVES)

test-rca:	test_rca.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-rca test_rca.c $(PRIMITIVES)

test-join:	test_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-join test_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-batched-join:	test_batched_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-batched-join test_batched_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-select:	test_select.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-select test_select.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-relational:	test_relational.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-relational test_relational.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-conversion:	test_conversion.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-conversion test_conversion.c $(PRIMITIVES)

test-arithmetic-to-binary:	test_arithmetic_to_binary.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-arithmetic-to-binary test_arithmetic_to_binary.c $(PRIMITIVES)

test-sort:	test_sort.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-sort test_sort.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-distinct:	test_distinct.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-distinct test_distinct.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-group:	test_group_by.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-group test_group_by.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-group-join:	test_group_by_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-group-join test_group_by_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-group-min-max:	test_group_by_min_max.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-group-min-max test_group_by_min_max.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-in:	test_in.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-in test_in.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-mask:	test_mask.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-mask test_mask.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-ageq:	test_adjacent_geq.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-ageq test_adjacent_geq.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-q1:	test_q1.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-q1 test_q1.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-q2:	test_q2.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-q2 test_q2.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-q3:	test_q3.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-q3 test_q3.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-tpch-q4:	test_tpch_q4.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-tpch-q4 test_tpch_q4.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-tpch-q4-baseline:	test_tpch_q4_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-tpch-q4-baseline test_tpch_q4_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-tpch-q6:	test_tpch_q6.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-tpch-q6 test_tpch_q6.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-tpch-q13:	test_tpch_q13.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-tpch-q13 test_tpch_q13.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-tpch-q13-baseline:	test_tpch_q13_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-tpch-q13-baseline test_tpch_q13_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-tpch-q6-baseline:	test_tpch_q6_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-tpch-q6-baseline test_tpch_q6_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-qpwd:	test_qpwd.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-qpwd test_qpwd.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-qpwd-baseline:	test_qpwd_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-qpwd-baseline test_qpwd_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-qcredit:	test_qcredit.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-qcredit test_qcredit.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-qcredit-baseline:	test_qcredit_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o test-qcredit-baseline test_qcredit_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

test-temp:	test-temp.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o test-temp test-temp.c $(PRIMITIVES)

test-server:	test_server.c 
			$(CC) $(CFLAGS) -o test-server test_server.c

test-client:	test_client.c
			$(CC) $(CFLAGS) -o test-client test_client.c 

test-party:	test_party.c $(PRIMITIVES) $(BASELINE)
			$(CC) $(CFLAGS) -o test-party test_party.c $(PRIMITIVES) $(BASELINE)

clean:
			rm -f test-init test-conn test-tpch-q6 test-conversion test-primitives test-rsz test-share test-select test-rca test-and-all test-equality test-relational test-group test-distinct test-sort test-inequality test-join test-in test-mask test-arithmetic-to-binary test-ageq test-group-join test-group-min-max test-q1 test-q2 test-q3 test-tpch-q4 test-tpch-q4-baseline test-tpch-q13 test-tpch-q13-baseline test-qpwd test-qpwd-baseline test-qcredit test-qcredit-baseline test-server test-client test-party *.o *.exec
