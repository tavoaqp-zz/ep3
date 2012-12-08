#include <new>
#include <stddef.h>
#include "PairCommObject.h"

PairCommObject::PairCommObject (int f, int s)
{
  first = f;
  second = s;
}

PairCommObject::PairCommObject (PairCommObject &another)
{
  first = another.first;
  second = another.second;
}

void PairCommObject::copyToArray (char *target, int offset)
{
  *((int *) (target + offset)) = first;
  *((int *) (target + offset + sizeof (int))) = second;
}

void PairCommObject::copyFromArray (char *target, int offset)
{
  first = *((int *) (target + offset));
  second = *((int *) (target + offset + sizeof (int)));
}

CommObject *PairCommObject::clone (void *p)
{
  if (p == NULL)
  {
    return new PairCommObject (*this);
  }
  else
  {
    return new (p) PairCommObject (*this);
  }
}

CommObject &PairCommObject::operator= (CommObject &another)
{
  PairCommObject *theOther = dynamic_cast <PairCommObject *> (&another);
  if (theOther == NULL)
  {
    throw ("Cannot copy from a non-PairCommObject into a PairCommObject");
  }

  first = theOther->first;
  second = theOther->second;
  return (*this);
}

int PairCommObject::getFirst (void)
{
  return first;
}

int PairCommObject::getSecond (void)
{
  return second;
}

void PairCommObject::setFirst (int f)
{
  first = f;
}

void PairCommObject::setSecond (int s)
{
  second = s;
}

void PairCommObject::sendToOstream (ostream &outstream)
{
  outstream << "(" << first << "," << second << ")";
}

int PairCommObject::getSize ()
{
  int result1 = sizeof (int) * 2;
  int result2 = sizeof (PairCommObject);
  return ((result1 > result2) ? result1 : result2);
}
