#!/bin/sh
mpiexec.hydra -f /mnt/data/hostfile -np 1 /mnt/data/cgmlib/applications/matrix/MatrixMultiplication 1,2,3,4

