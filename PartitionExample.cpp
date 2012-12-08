#include <stdlib.h>

#include "CGMTimers.h"
#include "Comm.h"
#include "GeneralUtilities.h"
#include "PairSumOperator.h"
#include "PairComparator.h"
#include "ParallelPrefixSummer.h"
#include "ParallelSorter.h"
#include "utils.h"
#include "PairCommObject.h"

int main (int argc, char **argv)
{
  CGMTimers *timers = new CGMTimers ();
  Comm *comm = Comm::getComm (&argc, &argv, timers);
  int n = -1;
  int p = comm->getNumberOfProcessors ();
  int id = comm->getMyId ();
  int groupId = id % 2;

  if (argc > 1)
  {
    n = atoi (argv [1]);
  }

  if (n <= 0)
  {
    n = 1000000; // default to 1 million data. 
  }

  int q = n / p;
  int r = n % p;

  if (id < r)
  {
    q++;
  }

  PairCommObject sample (0, 0);
  CommObjectList data (&sample);
  CommObjectList results (&sample);
  generateDataList (data, q, id);

  Operator *prefixOperator = new PairSumOperator ();
  ParallelPrefixSummer *prefixSummer = new ParallelPrefixSummer (comm, prefixOperator);
  Comparator *comp = new PairComparator ();
  ParallelSorter *sorter = new ParallelSorter (comp, comm);

//  GeneralUtilities::writeDataToFile (data, "PartitionData", "in.txt", id);

  comm -> synchronize ();
  timers->timersStart ();
  comm->partitionCGM (groupId);
  if (groupId == 0) // first group
  {
    sorter->sort (data);
  }
  else // second group
  {
    prefixSummer->calculatePrefixSum (results, data);
  }
  comm->unPartitionCGM ();
  timers->timersStop ();
  if (groupId == 0)
  {
//    GeneralUtilities::writeDataToFile (data, "PartitionData", "out.txt", id);
  }
  else
  {
//    GeneralUtilities::writeDataToFile (results, "PartitionData", "out.txt", id);
  }
  GeneralUtilities::printAverageTimes (timers, comm);

  comm->dispose ();

  delete (sorter);
  delete (comp);
  delete (prefixSummer);
  delete (prefixOperator);
  delete (timers);
}
