import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import subprocess
import image

subprocess.call(['run-exp-equality.sh'])
subprocess.call(['run-exp-equality-tcp.sh'])

text_file1 = open("mpi_timing.txt", "r")
MPI_data = text_file1.readlines()
text_file2 = open("tcp_timing.txt", "r")
TCP_data = text_file2.readlines()

num_elements = [1000, 10000, 100000, 1000000, 10000000]

plt.plot(num_elements, MPI_data, color='blue')
plt.plot(num_elements,TCP_data, color='red')
plt.savefig('timing_data.png')
