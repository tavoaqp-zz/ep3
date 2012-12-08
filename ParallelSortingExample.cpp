#include <stdlib.h>

#include "CGMTimers.h"
#include "Comm.h"
#include "GeneralUtilities.h"
#include "PairComparator.h"
#include "ParallelSorter.h"
#include "Debug.h"
#include "utils.h"
#include "PairCommObject.h"

#define NUM_TESTS 10

static void copyCommObjects (CommObjectList &target, CommObjectList &source)
{
  int n = source.getSize ();
  target.setSize (0);

  for (int i = 0; i < n; i++)
  {
    target.expand (source [i]);
  }
}

static void checkResult (CommObjectList &data, Comparator *comp, Comm *comm)
{
  CommObjectList copy (data);
  comm->allToOneGather (Comm::PROC_ZERO, copy);
  bool result = true;

  if (comm->getMyId () == Comm::PROC_ZERO)
  {
    int i;
    int n = copy.getSize ();

    for (i=1; i<n; i++)
    {
      if (comp->compare (copy [i-1], copy [i]) == 1)
      {
        result = false;
        break;
      }
    }

    cout << "The result is " << (result?"":"not ") << "correct.\n";
    cout.flush ();
  }
}

int main (int argc, char **argv)
{
//  Debug::setDebugStatus (true);
  CGMTimers *timers = new CGMTimers ();
  Comm *comm = Comm::getComm (&argc, &argv, timers);
  int n = -1;
  int p = comm->getNumberOfProcessors ();
  int id = comm->getMyId ();

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
  generateDataList (data, q, id);

  Comparator *comp = new PairComparator ();
  ParallelSorter *sorter = new ParallelSorter (comp, comm);

//  GeneralUtilities::writeDataToFile (data, "ParallelSortData", "in.txt", id);
  CommObjectList d (data.getDefaultData ());

  for (int i = 0; i < NUM_TESTS; i++)
  {
    copyCommObjects (d, data);
    comm -> synchronize ();
    timers->timersStart ();
    sorter->sort (d);
    timers->timersStop ();
  }
//  checkResult (data, comp, comm);
//  GeneralUtilities::writeDataToFile (data, "ParallelSortData", "out.txt", id);
  GeneralUtilities::printAverageTimes (timers, comm, NUM_TESTS);

  comm->dispose ();

  delete (sorter);
  delete (comp);
  delete (timers);
}
