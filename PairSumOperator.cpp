#include "PairSumOperator.h"
#include "PairCommObject.h"

void PairSumOperator::operate (CommObject *result, CommObject *first, CommObject *second)
{
  PairCommObject *f = dynamic_cast <PairCommObject *> (first);
  PairCommObject *s = dynamic_cast <PairCommObject *> (second);
  PairCommObject *r = dynamic_cast <PairCommObject *> (result);

  r->setFirst ((f->getFirst () + s->getFirst ()) % 1000);
  r->setSecond ((f->getSecond () + s->getSecond ()) % 1000);
}
