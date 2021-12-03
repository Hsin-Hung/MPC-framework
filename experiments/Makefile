CC= gcc -std=c99
CFLAGS= -O3 -Wall -lsodium -lm
# MPI= mpicc
SRC=../src
PRIMITIVES= $(SRC)/comm.c $(SRC)/primitives.c $(SRC)/mpc_tcp.c $(SRC)/utils.c $(SRC)/party.c $(SRC)/sharing.c
RELATIONAL= $(SRC)/relational.c
BASELINE= $(SRC)/baseline.c

all:	exp-equality exp-cmp-swap exp-exchange exp-reveal exp-greater exp-rca exp-distinct exp-join exp-semi-join exp-order-by exp-group-by exp-q1 exp-q2 exp-q3 exp-tpch-q4 exp-tpch-q4-baseline exp-tpch-q13 exp-tpch-q13-baseline exp-qpwd exp-qpwd-baseline exp-qcredit exp-qcredit-baseline

exp-equality:	exp_equality.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o exp-equality exp_equality.c $(PRIMITIVES)

exp-cmp-swap:	exp_comp_swap.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o exp-cmp-swap exp_comp_swap.c $(PRIMITIVES)

exp-exchange:	exp_exchange.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o exp-exchange exp_exchange.c $(PRIMITIVES)

exp-reveal:	exp_reveal.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o exp-reveal exp_reveal.c $(PRIMITIVES)

exp-greater:	exp_greater.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o exp-greater exp_greater.c $(PRIMITIVES)

exp-rca:	exp_rca.c $(PRIMITIVES)
			$(CC) $(CFLAGS) -o exp-rca exp_rca.c $(PRIMITIVES)

exp-distinct:	exp_distinct.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-distinct exp_distinct.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-join:	exp_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-join exp_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-semi-join:	exp_semi_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-semi-join exp_semi_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-order-by:	exp_order_by.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-order-by exp_order_by.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-group-by:	exp_group_by.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-group-by exp_group_by.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-group-by-baseline:	exp_group_by_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-group-by-baseline exp_group_by_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-q1:	exp_q1.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-q1 exp_q1.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-q1-baseline:	exp_q1_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-q1-baseline exp_q1_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-q2:	exp_q2.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-q2 exp_q2.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-q2-baseline:	exp_q2_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-q2-baseline exp_q2_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-q3:	exp_q3.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-q3 exp_q3.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-q3-baseline:	exp_q3_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-q3-baseline exp_q3_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-join-distinct-naive:	exp_join_distinct_naive.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-join-distinct-naive exp_join_distinct_naive.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-join-distinct-opt:	exp_join_distinct_opt.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-join-distinct-opt exp_join_distinct_opt.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-group-by-join-naive:	exp_group_by_join_naive.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-group-by-join-naive exp_group_by_join_naive.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-group-by-join-opt:	exp_group_by_join_opt.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-group-by-join-opt exp_group_by_join_opt.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-select-distinct-opt:	exp_select_distinct_opt.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-select-distinct-opt exp_select_distinct_opt.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-select-distinct-naive:	exp_select_distinct_naive.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-select-distinct-naive exp_select_distinct_naive.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-geq-join:	exp_geq_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-geq-join exp_geq_join.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-tpch-q4:	exp_tpch_q4.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-tpch-q4 exp_tpch_q4.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-tpch-q4-baseline:	exp_tpch_q4_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-tpch-q4-baseline exp_tpch_q4_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-tpch-q6:	exp_tpch_q6.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-tpch-q6 exp_tpch_q6.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-tpch-q13:	exp_tpch_q13.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-tpch-q13 exp_tpch_q13.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-tpch-q13-baseline:	exp_tpch_q13_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-tpch-q13-baseline exp_tpch_q13_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-qpwd:	exp_qpwd.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-qpwd exp_qpwd.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-qpwd-baseline:	exp_qpwd_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-qpwd-baseline exp_qpwd_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-qcredit:	exp_qcredit.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-qcredit exp_qcredit.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-qcredit-baseline:	exp_qcredit_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-qcredit-baseline exp_qcredit_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

exp-tpch-q6-baseline:	exp_tpch_q6_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)
			$(CC) $(CFLAGS) -o exp-tpch-q6-baseline exp_tpch_q6_baseline.c $(PRIMITIVES) $(RELATIONAL) $(BASELINE)

clean:
			rm -f exp-equality exp-cmp-swap exp-exchange exp-reveal exp-greater exp-rca exp-distinct exp-join exp-semi-join exp-order-by exp-group-by exp-q1 exp-q2 exp-q3 exp-tpch-q4 exp-tpch-q4-baseline exp-tpch-q13 exp-tpch-q13-baseline exp-qpwd exp-qpwd-baseline exp-qcredit exp-qcredit-baseline exp-group-by-join-naive *.o *.exec
