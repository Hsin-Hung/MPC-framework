# SysX

SysX is a relational Multi-Party Computation framework based on replicated secret sharing.

This repository is organized as follows:
- The `src` folder contains the core functionality of Sysx, including the implementation of MPC primitives, relational oblivious operators, and party communication.
- The `examples` folder contains the implementation of example queries with the SysX API.
- The `test` folder contains various unit and end-to-end tests.
- The `experiments` folder contains the implementation of various microbenchmarks and performance experiments.
- Plotting scripts and other helper utilies are located in the `scripts` folder.
- Further documentation and detailed instructions for a setting up a cloud-based SysX depoyment are located in `docs`.

Build SysX
============
The following instructions assume a single-node OSX system. See [below](#specifying-dependencies-on-linux) for instructions on how to properly specify dependencies on Linux. To setup SysX on a Cloud environemnt, please see the [Cloud setup instructions](docs/MPI-setup.md).

To build SysX, you will need to install:
- C99
- [Libsodium](https://libsodium.gitbook.io/doc/installation)
- an MPI implementation, such as [OpenMPI](https://www.open-mpi.org/software/ompi/v4.0/) or [MPICH](https://www.mpich.org/downloads/).

Build and run the tests
------------
Change to the `tests` directory.

1. Build and run all tests: 
   - Run `make tests`. 

2. Build and run an individual test: 
   - Run `make test-xyz` to build a test, where `xyz` is the test name. For instance, run `make test-equality` to build the binary equality test. 
   - Execute the test with `mpirun -np 3 test-xyz`.

Run an example
---------
Change to the `examples` directory.

1. Build all examples: 
   - Run `make all`. 

2. Build and run an individual example, e.g. the comorbidity query: 
   - Build the example with `make comorbidity`.
   - Run the example with `mpirun -np 3 comorbidity <NUM_ROWS_1> <NUM_ROWS_2>`.

Run the experiments
---------
Change to the `experiments` directory.

1. Build all experiments: 
   - Run `make all`. 

2. Build and run an individual experiment, e.g. the equality microbenchmark: 
   - Build the experiment with `make exp-equality`.
   - Run it with `mpirun -np 3 exp-equality <INPUT_SIZE>`.

Specifying dependencies on Linux
-------------
To build and run SysX on linux, edit the provided `Makefile` as follows:
- Use the variables `CFLAGS= -03 -Wall` and `DEP= -lsodium -lm`
- Specify the dependency in the end of the target, for example:

    `exp-equality:   exp_equality.c $(PRIMITIVES) $(MPI) $(CFLAGS) -o exp-equality exp_equality.c $(PRIMITIVES) $(DEP)`
