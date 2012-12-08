#!/bin/sh

runATest () # name <p> <nG> <nV> <nE>
{
  for i in 1
  do
    echo -n "Current Wall Clock: "
    date
    echo \<${i}\> Running ... mpirun -np $2 $1 $3
    mpiexec.hydra -f /mnt/data/hostfile -np $2 `pwd`/$1 $3
    rm -f *.txt
    echo
  done
}

runTests () # name
{
  for i in 1 2 3 4 6 8 12 16 24 32 48 64 96
  do
    runATest $1 ${i} 1000000
  done

  for i in 1000 2000 5000 10000 20000 50000 100000 200000 500000 1000000
  do
    runATest $1 8 ${i}
  done
}

for i in HRelationExample ParallelPrefixSumExample ParallelSortingExample
do
  runTests ${i}
done
