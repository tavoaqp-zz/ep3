#ifndef __PAIRCOMMOBJECT__
#define __PAIRCOMMOBJECT__

#include "CommObject.h"

class PairCommObject: public CommObject
{
  public:
    // constructors
    PairCommObject (int f, int s);
    PairCommObject (PairCommObject &another);

    // interface methods from CommObject
    virtual void copyToArray (char *target, int offset);
    virtual void copyFromArray (char *target, int offset);
    virtual CommObject *clone (void *p = NULL);
    virtual CommObject &operator= (CommObject &another);
    virtual void sendToOstream (ostream& outstream);
    virtual int getSize ();

    // non-interface methods
    int getFirst (void);
    int getSecond (void);
    void setFirst (int f);
    void setSecond (int s);

    // destructor
    virtual ~PairCommObject (void) {}

  private:
    int first;
    int second;
};

#endif
