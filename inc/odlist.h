
#ifndef ODList_H
#define ODList_H

/*
 *    $RCSfile: odlist.h,v $
 *    $Revision: 1.2 $
 *    $Date: 2002/09/30 20:55:21 $
 */

/////////////////////////////////////////////////////////////////////////
//
//  ODList.h : oredered double-linked list of objects  
//
/////////////////////////////////////////////////////////////////////////

#include "compable.h"
#include "olist.h"

class ODList_iterator;

class ODLink : public Object {

protected: 

  ODLink     *next;
  ODLink     *prev;
  Comparable *data;

public:

  ODLink() {;}
  ODLink(Comparable &d, ODLink *p, ODLink *n) { data=&d; prev=p; next=n;}
  ODLink(Comparable *d, ODLink *p, ODLink *n) { data= d; prev=p; next=n;}
 ~ODLink() {;}             

  friend class ODList;
  friend class ODList_iterator;
};

/////////////////////////////////////////////////////////////////////////
//

class ODList : public Object {

protected:
  ODLink  *head;
  ODLink  *tail;
  integer  count;

public:
  ODList() { head=0; tail=0; count=0;}
  ODList( List&);
  ODList( OList& l);
  ODList( ODList& l);
 ~ODList() { Clear();}

  // Adds object to list in order
  void Insert(Comparable&);
  void Insert(Comparable *o) { Insert(*o); }

  // Adds object in order of list
  // NOTE uniqueness of objects in ODList is determined by Compare()
  void InsertUnique(Comparable&);
  void InsertUnique(Comparable *o) { InsertUnique(*o); }

  void Union   ( ODList *l);       // Insert items if they are unique
  void Union   (  OList *l);       // Insert items if they are unique
  void Merge   (   List *l);        // Insert items into present list
  void Merge   ( ODList *l);       // Insert items into present list
  void Transfer( ODList *l);       // Inserts items, clearing l
  void Combine ( ODList *l ) { Transfer(l);}

  void    Clear();                   // Empties list
  void    CleanOut();                // Empties list and deletes objects
  boolean RemoveAllof(Comparable *); // Removes all items matching with Compare
  boolean Remove(Comparable *);      // Removes first item matches with Compare

  Comparable *Pop();                 // Removes & returns front element
  Comparable *Top();                 // Returns front element
  Comparable* GetElement( integer i);// Returns ith object in list w/o deleting
  Comparable* operator[]( integer i) { return GetElement(i);}

  boolean     Member(Comparable *o); // Confirm's o's existance in list
  integer     Count()  {return count;}

  void            ListPrint( ostream& out = cout, char *delim = "\n");
  virtual void    Print( ostream& out = cout);
  virtual integer isa() {return ODLIST;}

  friend class ODList_iterator;
};

/////////////////////////////////////////////////////////////////////////

class ODList_iterator : public Object {

  ODLink *ptr;
  OList_Order order;

public:

  ODList_iterator(ODList& l, OList_Order o)  { ptr = (o==Ascend)?  l.head: l.tail; order = o; }
  ODList_iterator(ODList* l, OList_Order o)  { ptr = (o==Ascend)? l->head:l->tail; order = o; }
 ~ODList_iterator()                  { ;}
  void        operator ++()          { ptr = ptr->next;}
  void        operator --()          { ptr = ptr->prev;}
  void          next()
  {
    // Changed for g++ compatibility
    if (order==Ascend)
      ptr=ptr->next;
    else
      ptr=ptr->prev;
  }

  boolean     end()                 { return (ptr == 0);}
  boolean     last()                 { return (ptr->next == 0);}
  boolean     first()                { return (ptr->prev == 0);}
  Comparable *item()                 { return ptr->data;}
  boolean     MoveTo( Comparable& c) { return MoveTo( &c);}
  boolean     MoveTo( Comparable*);

  void remove_d(ODList& x) { 
    ODLink *tmp = ptr; 
    ptr = ptr->prev; 
    delete tmp; 
    x.tail = ptr; --x.count;
  }

  void remove_a(ODList& x) { 
    ODLink *tmp = ptr; 
    ptr = ptr->next; 
    delete tmp; 
    x.head = ptr; --x.count;
  }
};
////////////////////////////////////////////////////////////////////////////

#define ODList_ForEach_Object_Descend(x, o) \
        { \
      Object *o; \
      for ( ODList_iterator odlist__itr(x,Descend); !odlist__itr.end(); --odlist__itr) { \
            o = odlist__itr.item();

#define ODList_ForEach_Type_Descend(x, t, o) \
        { \
      t *o; \
      for ( ODList_iterator odlist__itr(x,Descend); !odlist__itr.end(); --odlist__itr) { \
            o = (t *) odlist__itr.item();

#define ODList_ForEach_Object_Remove_Descend(x, o) \
        { \
      Object *o; \
      for ( ODList_iterator odlist__itr(x,Descend); !odlist__itr.end(); \
            odlist__itr.remove_d(x)) { \
            o = odlist__itr.item();

#define ODList_ForEach_Type_Remove_Descend(x, t, o) \
        { \
      t *o; \
      for ( ODList_iterator odlist__itr(x,Descend); !odlist__itr.end(); \
            odlist__itr.remove_d(x)) { \
            o = (t *) odlist__itr.item();

#define ODList_ForEach_Object_Ascend(x, o) \
        { \
      Object *o; \
      for ( ODList_iterator odlist__itr(x,Ascend); !odlist__itr.end(); ++odlist__itr) { \
            o = odlist__itr.item();

#define ODList_ForEach_Type_Ascend(x, t, o) \
        { \
      t *o; \
      for ( ODList_iterator odlist__itr(x,Ascend); !odlist__itr.end(); ++odlist__itr) { \
            o = (t *) odlist__itr.item();

#define ODList_ForEach_Object_Remove_Ascend(x, o) \
        { \
      Object *o; \
      for ( ODList_iterator odlist__itr(x,Ascend); !odlist__itr.end(); \
            odlist__itr.remove_a(x)) { \
            o = odlist__itr.item();

#define ODList_ForEach_Type_Remove_Ascend(x, t, o) \
        { \
      t *o; \
      for ( ODList_iterator odlist__itr(x,Ascend); !odlist__itr.end(); \
            odlist__itr.remove_a(x)) { \
            o = (t *) odlist__itr.item();

#define ODList_End_ForEach }}

////////////////////////////////////////////////////////////////////////////
#endif //ODLiST_H
