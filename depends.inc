BroadCastExample.o: BroadCastExample.cpp \
  utils.h \
  PairCommObject.h

HRelationExample.o: HRelationExample.cpp \
  utils.h \
  PairCommObject.h

PairCommObject.o: PairCommObject.cpp \
  PairCommObject.h

PairComparator.o: PairComparator.cpp \
  PairComparator.h \
  PairCommObject.h

PairSumOperator.o: PairSumOperator.cpp \
  PairSumOperator.h \
  PairCommObject.h

ParallelPrefixSumExample.o: ParallelPrefixSumExample.cpp \
  PairSumOperator.h \
  utils.h \
  PairCommObject.h

ParallelSortingExample.o: ParallelSortingExample.cpp \
  PairComparator.h \
  utils.h \
  PairCommObject.h

PartitionExample.o: PartitionExample.cpp \
  PairSumOperator.h \
  PairComparator.h \
  utils.h \
  PairCommObject.h

SendReceiveExample.o: SendReceiveExample.cpp \
  utils.h \
  PairCommObject.h

utils.o: utils.cpp \
  utils.h \
  PairCommObject.h
