/*
*******************************************************


   Xiang-Dong (Sheldon) Tan, 1998 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 10/26/1998 -> %G%
*******************************************************
*/


/*
 *    $RCSfile: list.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/05/20 04:22:01 $
 */

#include <iostream.h>
#include <stdlib.h>
#include "list.h"

////////////////////////////////////////////////////////////////////////////

List::List(List& l) 
{
  if ((count = l.count)) 
  {
    Link *p, *q;
    head = p = new Link( l.head->data, 0);
    for ( q = l.head->next; q; p = p->next, q = q->next)
      p->next = new Link( q->data, 0);

  } 
  else
	  head = 0;
}

////////////////////////////////////////////////////////////////////////////

void
List::Add(Object& p) {                    // List add
  Link *l = new Link(p, head);
  head = l;
  ++count;
}

////////////////////////////////////////////////////////////////////////////

void
List::Append(Object& p) {

  if (head == 0) {
    head = new Link(p,0);
  } else {
    Link *q = head;  // Moved in front of for loop for g++ -- Hien
    for (; q->next; q=q->next) {;}
    q->next = new Link(p,0);
  }
  ++count;
}

boolean
List::AppendUnique(Object &p) {

  if (head == 0) {
    head = new Link(p,0);
    ++count;
    return TRUE;
  } else {
    Link *q = head;  // Moved in front of for loop for g++ -- Hien
    for (; q->next && q->data != &p; q=q->next);
    if (q->data != &p) {
      q->next = new Link(p,0);
      ++count;
      return TRUE;
    }
  }
  return FALSE;
}

////////////////////////////////////////////////////////////////////////////

// Top returns the head element
// Pop returns the head element and removes it from list

Object*  
List::Top() {
  return (count)? head->data: 0;
}

Object*
List::Pop() {

  if (count--) {
    Object *d = head->data;
    Link *tmp = head;            
    head = head->next;           // advance head
    delete tmp;                  // destroy Link;
    return d;
    
  } else
    FATAL ("Pop from empty list");
  return NULL;
}

Object*
List::GetElement (integer i) {
  if ( i<0 || i>=count) return 0;

  Link *p = head;
  for ( integer cnt = 0; cnt < i; cnt++) {
    p=p->next;
  } 
  return p->data;
}

integer
List::IdentityNum(Object *o) {
  integer pos = 0;
  for( Link *p = head; p ; p=p->next, pos++ )
    if( p->data == o)         // if pointers are the same
      return pos;
  return -1;
}

integer
List::ElementNum(Comparable *o) {
  integer pos = 0;
  for( Link *p = head; p ; p=p->next, pos++ )
    if( *(Comparable*)p->data == *o )
      return pos;
  return -1;
}

/////////////////////////////////

boolean
List::Member(Object *o) {

  for (Link *p = head; p; p=p->next) {
    if ( p && p->data == o) {
      return TRUE;
    }
  } 
  return FALSE;
}

boolean
List::Member(Comparable *o) {

  for (Link *p = head; p; p=p->next) {
    if ( p && *(Comparable *)p->data == *o) {
      return TRUE;
    }
  } 
  return FALSE;
}

Comparable*
List::GetMember(Comparable *o) {

  for (Link *p = head; p; p=p->next) {
    if (p && *(Comparable *)p->data == *o) {
      return (Comparable *) p->data;
    }
  } 
  return NULL;
}


////////////////////////////////////////////////////////////////////////////
// Remove all links in list, but maintains objects

void
List::Clear () {
  Link *d;
  while ( head) {
    d    = head;
    head = d->next;
    delete d;
  }
  count = 0;
}

////////////////////////////////////////////////////////////////////////////
// Remove all links in list, and deletes objects

void
List::CleanOut() {
  Link *d;
  while ( head) {
    d    = head;
    head = d->next;
    delete d->data;
    delete d;
  }
  count = 0;
}

////////////////////////////////////////////////////////////////////////////
// Remove all matching objects in list. Return TRUE if any removed.

boolean
List::RemoveAllof (Object *o) {

  Link   *p, *q, *n;
  boolean removed = FALSE;

  for (p = head, q=0; p; p=n) {
    n = p->next;
 
    if (p->data == o) {
      if (q) 
    q->next = p->next;
      else 
    head = p->next;
      delete p;
      count--;
      removed = TRUE;
     } else 
      q = p;
  } 
  return removed;
}

boolean
List::Remove(Object *o) {                // just remove one instance

  Link *point,*lastPoint;
  //boolean removed = FALSE;

  for ( point=head, lastPoint=NULL; point; point = point->next) {
    
    if (point->data == o) {
      if ( lastPoint) 
    lastPoint->next = point->next;
      else 
    head = point->next;
      delete point;
      count--;
      return TRUE;
    }
    lastPoint = point;
  } 
  return FALSE;
}


////////////////////////////////////////////////////////////////////////////
// List printer

void
List::Print( ostream& out) {
  if (count == 0) 
    out << "List : empty\n";
  else {
    out << "List : " << count << " item(s) ->\n";
    for (Link *p=head; p; p=p->next)
      out << p->data;
  }
}

void
List::ListPrint(ostream& out, char *delim) {
   for (Link *p=head; p; p=p->next) {
      out <<  p->data << delim;
  }
}

////////////////////////////////////////////////////////////////////////////

void
List::AddList(List *toadd) {
  List *list = new List( *toadd);
  Transfer( list);
  delete list;
}

void
List::AppendList(List *toadd) {
  List *list = new List( *toadd);
  TransferAppend( list);
  delete list;
}

void
List::Transfer(List *toadd) {

  if ( toadd->head == 0)  
    return;
  Link* p = toadd->head;  // Moved in front of for loop for g++ -- Hien
  for ( ; p->next; p = p->next);
  p->next = head;
  head    = toadd->head;
  count  += toadd->count;

  toadd->head  = 0;
  toadd->count = 0;
}

void
List::TransferAppend(List *toadd) {

  if ( head) {
    Link* p = head;  // Moved in front of for loop for g++ -- Hien
    for ( ; p->next; p = p->next);
    p->next = toadd->head;
  } else {
    head    = toadd->head;
  }
  count += toadd->count;

  toadd->head  = 0;
  toadd->count = 0;
}

void
List::Union(List *toadd) {

  for ( Link* p = toadd->head; p; p = p->next)
    AppendUnique( p->data);
}  

////////////////////////////////////////////////////////////////////////////

void
List::Reverse() {
  Link *t, *temp = head;
  head = 0;
  while ( temp) {
    t = temp->next;
    temp->next = head;
    head = temp;
    temp = t;
  }
}
