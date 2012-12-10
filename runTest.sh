#!/bin/sh
mpiexec.hydra -f /mnt/data/hostfile -np 4 /mnt/data/cgmlib/applications/matrix/MatrixMultiplication 2,5,1,3,6

