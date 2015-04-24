#ifndef OBJECT_H
#define OBJECT_H

/*
 *    $RCSfile: object.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

////////////////////////////////////////////////////////////////////////////
//
//  object.h : Object class 
//
////////////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include <stdlib.h>
#include "stddefs.h"
#include "classdef.h"
#include "copyr.h"

#include "mem.h"
#define NO_MEM

//extern MemGroup mem_default_group;

//////////////////////////////////////////////////////////////

//
// Object is a virtual base class
//

#ifndef NO_MEM
#define MEM
#endif

class Object
{   
public:
  Object() {;}
  virtual ~Object() {;}

  // Print is, by convention, expected to print debugging info.
  // For pretty printing redefine the << operator.
  virtual void Print(ostream &out = cout)
  { out << "ERROR: write a Print funct!\n";}

  virtual void Scan(istream &in = cin)
  { if (in.eof()); /* just uses in */ cout << "ERROR: write a Read funct!\n";}
     
  virtual boolean isComparable() { return FALSE;} 
  virtual boolean isSticky()     { return FALSE;} 
  virtual integer isa()          { return OBJECT;}
  virtual boolean Flush()     { return TRUE; }
  // This is more general than a constructor since constructors
  // are not virtual 
  virtual Object *Copy()
  { cerr << "Object has no Copy() member\n"; return(NULL); }

#ifdef MEM
  void *operator new (size_t n) { return mem_default_group.Malloc( n ); }
  void  operator delete (void *p, size_t n)
    { mem_default_group.Free(p, n); }
#endif

  // Prevent annoying warning from appearing
  void antiWarning()        { char *c=module_copyright_notice; c=c; }
};

class DoubleObject : public Object {

    double num;

    public:
    DoubleObject( double val ) { num = val; }
    ~DoubleObject() {;}
    double get_value(void) { return num; }
};

inline istream&
operator>>(istream &in,Object *o)
{
  o->Scan(in);
  return(in);
}

inline istream&
operator>>(istream &in,Object &o)
{
  o.Scan(in);
  return(in);
}

inline ostream&
operator<<(ostream &out, Object &o)
{
  o.Print(out);
  return out;
}

inline ostream&
operator<<(ostream &out, Object *o)
{
  o->Print (out);
  return out;
}

#endif // OBJECT_H

