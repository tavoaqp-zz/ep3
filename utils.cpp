#include <iomanip.h>
#include <iostream.h>
#include "utils.h"
#include "PairCommObject.h"
#include "Random.h"
#include "GeneralUtilities.h"

void generateDataList (CommObjectList &data, int n, int seed)
{
  int i;
  Random random (seed);

  PairCommObject *currentObject;

  data.setSize (n);

  for (i=0; i<n; i++)
  {
    currentObject = dynamic_cast <PairCommObject *> (data [i]);
    currentObject -> setFirst (random.getInteger (1000));
    currentObject -> setSecond (random.getInteger (1000));
  }
}
