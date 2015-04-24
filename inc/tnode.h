#ifndef TNODE_H
#define TNODE_H

/*
 *    $RCSfile: tnode.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#include <iostream.h>
#include "compable.h"
#include "classdef.h"

#define MARK_BIT (unsigned short) 0x8000

class Tnode : public Comparable {

protected:
  Tnode        *left;
  Tnode           *right;
  unsigned short    reference;
  short            index;

  void    SetLeft(Tnode *l)  { left  = l;}
  void    SetRight(Tnode *r) { right = r;}

public:
  Tnode() { left = right = NULL; reference=0; }
 ~Tnode() {;}

  Tnode*  Left()          { return left;}
  Tnode*  Right()         { return right;}
  Tnode*  One()           { return left;}
  Tnode*  Zero()          { return right;}
//  Tnode*  True()          { return left;}
//  Tnode*  False()         { return right;}

  virtual void    Reference()     { reference++;          }
  virtual void    UnReference()   { reference--;          }
  virtual unsigned NumReferences() { return reference & ~MARK_BIT; }

  void    Mark()          { reference |= MARK_BIT;    }
  void    UnMark()        { reference &= ~MARK_BIT;    }
  boolean Marked()        { return reference & MARK_BIT;}

  virtual void Print(ostream& out = cout) { out << "Tnode\n"; } 
};

inline ostream& operator << (ostream &out, Tnode& f)
{ f.Print(out); return out; }

inline ostream& operator << (ostream &out, Tnode* f)
{ out << "(ptr->)"; f->Print(out); return out; }

#endif // TNODE_H

