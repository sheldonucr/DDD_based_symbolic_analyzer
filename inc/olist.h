
#ifndef OList_H
#define OList_H

/*
 *    $RCSfile: olist.h,v $
 *    $Revision: 1.2 $
 *    $Date: 2002/09/30 20:55:21 $
 */


/////////////////////////////////////////////////////////////////////////
//
//  OList.h : ordered linked list of objects  
//
/////////////////////////////////////////////////////////////////////////

#include "compable.h"
#include "list.h"

class OList_iterator;

class OLink : public Object {

protected: 

  OLink      *next;
  Comparable *data;

public:

  OLink() {;}
  OLink(Comparable &d, OLink *n) { data=&d; next=n;}
  OLink(Comparable *d, OLink *n) { data=d; next=n;}
 ~OLink() {;}             

  friend class ODList;
  friend class  OList;
  friend class  OList_iterator;
};

/////////////////////////////////////////////////////////////////////////
//

enum OList_Order{ Ascend=1, Descend=-1};

class OList : public Comparable {

protected:
  OLink      *head;
  integer     count;
  integer     hash_val;
  OList_Order type;

public:
  OList( OList_Order t = Ascend) { head=0; count=0; type=t; hash_val=0;}
  OList( List&, OList_Order t = Ascend);
  OList( OList& l);
 ~OList() { Clear();}

  // Adds object to list in order
  void Insert(Comparable& c);
  void Insert(Comparable* c) { Insert(*c); }

  // Adds object in order of list
  // NOTE uniqueness of objects in OList is determined by Compare()
  // returns whether the value was actually inserted
  boolean InsertUnique(Comparable& c);
  boolean InsertUnique(Comparable* c) { return InsertUnique(*c); }

  void Union   ( OList* l, boolean alloc_mem = FALSE);   // Insert items if they are unique
  void Merge   (  List* l, boolean alloc_mem = FALSE);   // Insert items into present list
  void Merge   ( OList* l, boolean alloc_mem = FALSE);   // Insert items into present list
  void Transfer( OList* l);                              // Inserts items, clearing l
  void Combine ( OList* l ) { Transfer( l);}

  void    Clear();                   // Empties list
  void    CleanOut();                // Empties list and deletes objects
  boolean RemoveAllof(Comparable *); // Removes all items matching with Compare
  boolean Remove(Comparable *);      // Removes first item matches with Compare

  OList*      Diff( OList* rem);     // Returns list of elements in this, but not in rem

  Comparable* Pop();                 // Removes & returns front element
  Comparable* Top();                 // Returns front element
  Comparable* GetElement( integer i);// Returns ith object in list w/o deleting
  Comparable* operator[]( integer i) { return GetElement(i);}
  Comparable* GetLast()              { return GetElement( count-1);}

  Comparable* GetEquiv(Comparable* c); // returns element which is equivalent to c
  boolean     Member(Comparable* c) {  // Confirm's o's existance in list
                                       return GetEquiv(c) != NULL;}
  integer     Count()  {return count;}
  void        Reverse();
  OList_Order Order()    { return type;}
  OList_Order OppOrder() { return (OList_Order) (-1*type);}

  virtual integer  Compare( Comparable*);
  virtual unsigned Hash( unsigned i=0) { return hash_val+i*4717;}

  void            ListPrint( ostream& out = cout, char *delim = "\n");
  virtual void    Print( ostream& out = cout);
  virtual integer isa() {return OLIST;}

  friend class ODList;
  friend class  OList_iterator;
};

/////////////////////////////////////////////////////////////////////////

class OList_iterator : public Object {

  OLink *ptr;

public:

  OList_iterator(OList& l)  { ptr = l.head;}
  OList_iterator(OList* l)  { ptr = l->head;}
 ~OList_iterator()          { ;}
  void        operator ++() { ptr = ptr->next;}
  boolean     end()        { return (ptr == 0);}
  boolean     last()        { return (ptr->next == 0);}
  Comparable *item()        { return ptr->data;}

  void remove(OList& x) { 
    OLink *tmp = ptr; 
    ptr = ptr->next; 
    delete tmp; 
    x.head = ptr; --x.count;
  }
  void remove(OList* x) { remove( *x);}
};
////////////////////////////////////////////////////////////////////////////

#define OList_ForEach_Object(x, o) \
        { \
      Comparable *o; \
      for ( OList_iterator olist__itr(x); !olist__itr.end(); ++olist__itr) { \
            o = olist__itr.item();

#define OList_ForEach_Type(x, t, o) \
        { \
      t *o; \
      for ( OList_iterator olist__itr(x); !olist__itr.end(); ++olist__itr) { \
            o = (t *) olist__itr.item();

#define OList_ForEach_Object_Remove(x, o) \
        { \
      Comparable *o; \
      for ( OList_iterator olist__itr(x); !olist__itr.end(); \
            olist__itr.remove(x)) { \
            o = olist__itr.item();

#define OList_ForEach_Type_Remove(x, t, o) \
        { \
      t *o; \
      for ( OList_iterator olist__itr(x); !olist__itr.end(); \
            olist__itr.remove(x)) { \
            o = (t *) olist__itr.item();

#define OList_End_ForEach }}

////////////////////////////////////////////////////////////////////////////
#endif //OLIST_H
