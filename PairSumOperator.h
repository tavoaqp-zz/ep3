#ifndef __PAIRSUMOPERATOR__
#define __PAIRSUMOPERATOR__

#include "Operator.h"

class PairSumOperator: public Operator
{
  public:
    virtual void operate (CommObject *result, CommObject *first, CommObject *second);
};

#endif
