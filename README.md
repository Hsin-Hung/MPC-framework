## Building a Secure Communication Layer for Multi-Party Computing - Project Description (UPDATE: 10/18/2021)

## 1. Visions and Goals of the Project

Our communication layer will replace Message Passing Interface (MPI), the messaging protocol currently used in Secrecy:

  - Eliminate MPI dependency in Secrecy and establish standing TCP connections
  - Run our Secrecy prototype on a Linux Unikernel (UKL)

MPI is very effective for HPC, which happens on a single cluster. However, since MPC parties are not necessarily located in the same place, an communication protocol that uses the internet (i.e. TCP) is needed. Additionally, Secrecy is meant to eventually be deployed on a Unikernel, which MPI is not compatible with.

## 2. Users and Personas of the Project

In order for outside parties to benefit from MPC, developers will implementing this improved software. They will benefit from a faster communication layer that enables MPC computation of a data owner's sensitive data at an improved rate.

Developers of the MPC software with this faster communication layer will be the main users of this project as implementation on a unikernel rather than reliance of MPI communication will allow for faster computation.

This project does not target those outside parties (data owner and data learner) that are inputting and visualizing this sensitive data. They will not be interacting with the communication layer as that will be the task of the developers. They will, however, also benefit from faster computation speeds regarding their sensitive data. 

## 3. Scope and Features of the Project

- Remove dependencies on MPI
    - Replace MPI init with a sockets init function
    - Establish TCP connections between parties involved and orchestration mechanism
    - Asynchronous Communication
       - TCP is intrinsically asynchronous
    - Maintain proper function of all other aspects of current Secrecy framework

- Orchestration
    - Implement way to orchestrate party IP addresses for TCP
    - Master Orchestrator: spawns processes/waits for parties to contact it to pass IP addresses to other parties

- TCP
    - Network communication protocol that uses IP addresses and port number for routing
    - It rearranges data packets in the order specified with guarantee that they will be received in the same order sent
    - Does flow control
    - Programmed using C sys/socket library
    - To set up standard socket connection, three data network packets to set up the socket connection
    
    ![image](https://github.com/msisk23/MPC_Project/blob/main/TCP%20Flow%20Diagram.png)
    _**Figure 1: Flow Diagram of how a TCP Server operates.**_ 

- Unikernel deployment
    - Benchmarking of MPI alternative
    - Ability to run communication-intensive applications using our MPI alternative on a Linux Unikernel (UKL)
    - Report performance improvements when using UKL (if any)

- Unikernel
    - Our understanding is that we aren't going to worry about the UKL until after we have a functioning MPI solution, and then PhD students from Professor Krieger's group will help us run Secrecy on UKL.

## 4. Solution Concept
**Global Architectural Structure of the Project**

Crucial project components and definitions:
  - MPI: Message Passing Interface - commonly used in high performance computing, is very effective and efficient on a single cluster. Doesn't necessarily work with MPC due to physical locations of different parties, and is incompatible with UKL
  - Party: One of three web services used during the data transfer process. The "hub" where messages are sent or received.
  - Web: Cloud providers that provide machines were secure computations on supplied data are taking place.
  - Multi Party Communication (MPC): Communication between three cloud services to ensure secure data transmission and evaluation
  - Secrecy: Application used to securely analyze private data
  - Master Orchestrator: entity that receives IP addresses of parties and passes them to other parties to establish socket connections and open TCP flow
![image](https://github.com/msisk23/MPC_Project/blob/main/Diagram.jpg)
_**Figure 2: Architecture of the MPC. Black components currently in use, blue components to be implemented.**_

Figure 2 demonstrates the current structure of the MPC, and the structure to be implemented. Currently, MPI is used to enable communication between parties. During certain points of program execution, parties have to verify information with each other. In the current MPC implementation, parties can only communicate along the Main Thread one message at a time. In order to establish a non-blocking method of asynchronous verification, a Communication Thread (seen in blue) will replace MPI. Using TCP communication, input and output buffers will allow for the non-blocking transfer of data. Later, the data will be processed asynchronously to provide verification for each party. 

**Design Implications and Discussion**

Key Design Decisions and Implementations:
  - MPI Elimination: MPI was first deployed as a temporary solution. In order to deploy Secrecy for MPC, remove unnecessary software depenencies and run on UKL, MPI needs to be replaced.
      - This will be done by implementing TCP connections between parties
  - Addition of a Communication Thread: When two parties want to exchange messages through the main thread, it blocks all the main thread operations or computations. By dedicating parties communication tasks to additional threads, the parties will be able to pull from a communication thread buffer, instead of the main thread, which eliminates the block. In order to implement multithreading, we will be using pthreads allowing our group to maintain high speed communication without the MPI (currently not the focus of the project, will come up later during optimization).
  - Implementation of Buffers: When two parties want to exchange messages, they cannot do so asynchronously. As such, only one message can be processed at a time. With the addition of input and output buffers, parties will be able to send and pull messages without being in sync (Also part of optimization, for a later sprint).
  - Unikernel Implementation: After verifying functionality of the MPI-free system, MPC will run on top of a Unikernel. The stripped down implementation will further speed up MPC implementation. 


## 5. Acceptance Criteria

Minimum acceptance is defined as replacing MPI in Secrecy with functioning TCP connections and implementing a party orchestrator so that our solution can be tested on the MOC. Stretch goals include:
  - Implementing communication thread with input/outpur buffers for each party using pthreads (optimization)
  - Run a communication-intensive application using our Secrecy prototype on the UKL
  - Testing and benchmarking our prototype to compare performance gains against MPI performance
  
## 6. Release Planning
Release #1:

  - Remove dependencies on MPI from init function


Release #2:

  - Remove dependencies on MPI from init function
  - Establish standing TCP connection between 3 parties


Release #3:

  - Implement Master Orchestrator for socket connections in Secrecy
  - Implement TCP send/receive for data communication between parties
  - Remove other MPI dependencies


Release #4:

  - Test  to ensure implementation functions without MPI
  - Begin deployment on UKL
  - Test on UKL


Release #5:

  - Optimize using pthreads (communication thread & buffers)
  - Final testing
  - Deployment of solution and optimization on UKL

## DEMO 1

https://youtu.be/9HV23bVlr6E

## DEMO 2

https://youtu.be/NU8P3QJDxLk

## Mentors
John Liagouris: liagos@bu.edu

Professor Orran Krieger: okrieg@bu.edu

Professor Peter Desnoyers: pjd-nu or pjd@ccs.neu.edu

Anqi Guo: anqianqi1


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


## Demo 1

https://youtu.be/9HV23bVlr6E

## Demo 2

https://youtu.be/NU8P3QJDxLk
