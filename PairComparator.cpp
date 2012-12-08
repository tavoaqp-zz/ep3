#include <stddef.h>
#include "PairComparator.h"
#include "PairCommObject.h"

PairComparator::PairComparator (void)
{
}

bool PairComparator::isComparable (CommObject *object)
{
  PairCommObject *pPCO = dynamic_cast <PairCommObject *> (object);
  return pPCO != NULL;
}

static int compareInteger (int i1, int i2)
{
  if (i1 < i2)
    return -1;

  if (i1 > i2)
    return 1;

  // (i1 == i2)
    return 0;
}

int PairComparator::compare (CommObject *a, CommObject *b)
{
  if (!isComparable (a))
  {
    throw ("First object to compare is not comparable");
  }

  if (!isComparable (b))
  {
    throw ("Second object to compare is not comparable");
  }

  PairCommObject *aa = (PairCommObject *) a;
  PairCommObject *bb = (PairCommObject *) b;

  int result = compareInteger (aa->getSecond (), bb->getSecond ());

  if (result == 0)
  {
    return compareInteger (aa->getFirst (), bb->getFirst ());
  }
  else
  {
    return result;
  }
}
