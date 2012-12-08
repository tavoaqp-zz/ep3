#ifndef __PAIRCOMPARATOR__
#define __PAIRCOMPARATOR__

#include "Comm.h"
#include "Comparator.h"

class PairComparator: public Comparator
{
  public:
    PairComparator (void);
    virtual bool isComparable (CommObject *object);
    virtual int compare (CommObject *a, CommObject *b);
    virtual ~PairComparator (void) {}
};

#endif
