#include <stdlib.h>

#include "CGMTimers.h"
#include "Comm.h"
#include "GeneralUtilities.h"
#include "utils.h"
#include "PairCommObject.h"

#define NUM_TESTS 40

static void copyCommObjects (CommObjectList &target, CommObjectList &source)
{
  int n = source.getSize ();
  target.setSize (0);

  for (int i = 0; i < n; i++)
  {
    target.expand (source [i]);
  }
}

int main (int argc, char **argv)
{
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

  int *sizes = new int [p];

  int sq = q / p;
  int sr = q % p;

  int i;
  for (i=0; i<p; i++)
  {
    if (i < sr)
    {
      sizes [i] = sq + 1;
    }
    else
    {
      sizes [i] = sq;
    }
  }

//  GeneralUtilities::writeDataToFile (data, "HRelationData", "in.txt", id);
  CommObjectList d (data.getDefaultData ());

  for (int i = 0; i < NUM_TESTS; i++)
  {
    copyCommObjects (d, data);
    comm -> synchronize ();
    timers->timersStart ();
    comm->hRelation (d, sizes);
    timers->timersStop ();
  }
//  GeneralUtilities::writeDataToFile (data, "HRelationData", "out.txt", id);
  GeneralUtilities::printAverageTimes (timers, comm, NUM_TESTS);

  comm->dispose ();

  delete [] (sizes);
  delete (timers);
}
