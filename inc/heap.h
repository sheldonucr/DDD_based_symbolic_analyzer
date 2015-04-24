#ifndef HEAP_H
#define HEAP_H

/*
 *    $RCSfile: heap.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

//
// This is a Heap class header file.
//
// Description of individual functions can be found 
// in 'heap.C'.
//

#include <iostream.h>
#include "compable.h"


class Heap : public Object {

  Comparable **heap;
  integer tail;
  integer maxsize;

public:

  Heap(integer n);
 ~Heap() { delete heap; }

  Comparable*   DeleteMin();
  Comparable*   Pop();          
  void    Insert(Comparable& x);
  void    Add(Comparable& x);
  void    Add(Comparable *x);      
  void    Push(Comparable& x);   
  void    Push(Comparable *x); 
  integer HeapSize() { return(tail); } 
  integer MaxSize()  { return(maxsize); }

  virtual void Print(ostream& out = cout);
  virtual integer isa()           { return HEAP; }

};

inline ostream& operator << (ostream &out, Heap& f)
{ f.Print(out); return out; }

inline ostream& operator << (ostream &out, Heap* f)
{ out << "(ptr->)"; f->Print(out); return out; }

#endif // HEAP_H

