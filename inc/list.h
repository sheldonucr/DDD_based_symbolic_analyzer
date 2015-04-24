#ifndef LIST_H
#define LIST_H

/*
 *    $RCSfile: list.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

/////////////////////////////////////////////////////////////////////////
//
//  list.h : linked list of objects  
//
/////////////////////////////////////////////////////////////////////////

#include "object.h"
#include "compable.h"

class List_iterator;

class Link : public Object {
protected: 

  Link   *next;
  Object *data;

public:

  Link() {;}
  Link(Object &d, Link *n) { data=&d; next=n;}
  Link(Object *d, Link *n) { data=d; next=n;}
 ~Link() {;}

  Link *Next() { return next; }
  Object *Data() { return data; }

  friend class   List;
  friend class  OList;
  friend class ODList;
  friend class   List_iterator;
};

/////////////////////////////////////////////////////////////////////////
//

class List : public Object {
protected:
  Link   *head;
  integer count;

public:
  List(List& l);
  List() { head=0; count=0;}
 ~List() { Clear();}

  // Adds object to front 
  void Add(Object&);
  void Add(Object *o) { Add(*o); }

  // Appends object
  void Append(Object&);
  void Append(Object *o) { Append(*o); }

  // Appends object if unique
  // NOTE uniqueness of objects in List is determined by address
  boolean AppendUnique(Object&);
  boolean AppendUnique(Object *o) { return AppendUnique(*o); }

  void Union(  List *l);            // Appends the unique items from l
  void AddList(List *l);            // Add List l to front in order
  void AppendList(List *l);         // Append List l in order
  void Transfer(List *l);           // Adds List l to front in order, clearing l
  void TransferAppend(List *l);     // Appends List l in order, clearing l

  void Reverse();

  void    Clear();                  // Empties list
  void    CleanOut();               // Deletes objects as it empties list
  boolean RemoveAllof(Object *);    // Removes all items with same address
  boolean Remove(Object *);         // Removes first item with same address

  Object* Pop();                    // Removes & returns front element
  Object* Top();                    // Returns front element
  Object* GetElement( integer i);   // Returns ith object in list w/o deleting
  Object* operator[]( integer i)    { return GetElement(i);}

  integer IdentityNum(Object    *o); // return o's pos in list (or -1)
  integer ElementNum(Comparable *o); // return "equiv o"'s pos in list (or -1)
  boolean Member(Object *o);         // Confirm's o's existance in list
  boolean Member(Comparable *o);     // Confirm's o's existance in list
  Comparable* GetMember(Comparable *o); // Return the Object or NULL
  integer Count()  {return count;}
  void    ListPrint( ostream& out = cout, char *delim = "\n");
  virtual void Print( ostream& out = cout);
  virtual integer isa() {return LIST;}

  friend class  OList;
  friend class ODList;
  friend class   List_iterator;
};

inline ostream& operator << (ostream &out, List& f)
{ f.Print(out); return out; }

inline ostream& operator << (ostream &out, List* f)
{ out << "(ptr->)"; f->Print(out); return out; }

/////////////////////////////////////////////////////////////////////////

class List_iterator : public Object {

  Link *ptr;

public:

  List_iterator(List& l) { ptr = l.head;}
  List_iterator(List* l) { ptr = l->head;}
 ~List_iterator()        { ;}
  void operator ++()     { ptr = ptr->next;}
  int end()             { return (ptr == 0);}
  int last()             { return (ptr->next == 0);}
  Object *item()         { return (ptr)? ptr->data:0;}
  Object *nextItem()     { return (ptr)?((ptr->next)?ptr->next->data:0):0;}

  void remove(List& x) { 
    Link *tmp = ptr; 
    ptr=ptr->next; 
    delete tmp; 
    x.head=ptr; --x.count;
  }
  void remove(List* x) { remove( *x);}
};
////////////////////////////////////////////////////////////////////////////

#define List_ForEach_Object(x, o) \
        { \
      Object *o; \
      for ( List_iterator list__itr(x); !list__itr.end(); ++list__itr) { \
            o = list__itr.item();

#define List_ForEach_Type(x, t, o) \
        { \
      t *o; \
      for ( List_iterator list__itr(x); !list__itr.end(); ++list__itr) { \
            o = (t *) list__itr.item();

#define List_ForEach_TypeCast(x, t, o) \
        { \
      t o; \
      for ( List_iterator list__itr(x); !list__itr.end(); ++list__itr) { \
            o = (t) list__itr.item();

#define List_ForEach_Object_Remove(x, o) \
        { \
      Object *o; \
      for ( List_iterator list__itr(x); !list__itr.end(); \
            list__itr.remove(x)) { \
            o = list__itr.item();

#define List_ForEach_Type_Remove(x, t, o) \
        { \
      t *o; \
      for ( List_iterator list__itr(x); !list__itr.end(); \
            list__itr.remove(x)) { \
            o = (t *) list__itr.item();

#define List_End_ForEach }}

////////////////////////////////////////////////////////////////////////////

#endif // LIST_H

