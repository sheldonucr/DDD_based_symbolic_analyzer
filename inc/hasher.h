#ifndef HASHER_H
#define HASHER_H

/*
 *    $RCSfile: hasher.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#include "compable.h"
#include "list.h"

#define HasherSort   HasherTree( TRUE)
#define HasherNuke   HasherTree( TRUE, TRUE)

#define HASHER_MOD( x ){ x = (x & 2097143) + (x >> 21) + (x>> 18); \
                x = (x & 2097143) + (x >> 21) + (x>> 18); }

class Hasher_iterator;
class Hasher_updater;

class HasherNode : public Object {

  HasherNode    *right;
  HasherNode    *left;
  integer        hashval;
  Comparable    *key;
  Object        *data;
       

  HasherNode(integer, Comparable *, Object *);
  HasherNode(HasherNode *, boolean copy = TRUE); // recursive copy
 ~HasherNode() {;}

  Comparable *GetKey()         { return key;  }
  Object *Data()               { return data; }
  void Data (Object *d)        { data = d;    }
  void Key (Comparable *k)     { key  = k;    }

  HasherNode *Right()          { return right; }
  void Right (HasherNode *r)   { right = r; }
  HasherNode *Left()           { return left; }
  void Left (HasherNode *l)    { left = l; }
  integer Hashval()            { return hashval; }

  friend class HasherTree;
  friend class Hasher_iterator;
  friend class Hasher_updater;
};

////////////////////////////////////////////////////////////////////////////

class HasherTree : public Object {

  friend class Hasher_iterator;
  friend class Hasher_updater;
       
  virtual void PrintNode(HasherNode *, int, ostream& out = cout);
  void    CheckLevel(HasherNode *, int);

protected:

  integer count;
  HasherNode *root;
  struct {
    unsigned int max_depth    : 30;
    boolean    del_data    :  1;
    boolean    del_key        :  1;
  } flags;

  boolean DelData() { return flags.del_data;}
  boolean DelKey () { return flags.del_key;}
       
public:

  HasherTree( boolean del_data = FALSE, boolean del_key = FALSE);
  HasherTree( HasherTree *, boolean copy=FALSE);
 ~HasherTree()        { if (root != 0) Destroy(root);}
  integer Count ()     { return count;}

  void SetDelete(boolean d, boolean k) { flags.del_data = d; flags.del_key = k; }

  virtual void Destroy(HasherNode *n );
  void Destroy()    { if( root )  {Destroy(root); root = NULL;} }

  boolean Put (Object &o, Comparable *key);         // returns TRUE if successful
  boolean Put (Object *o, Comparable *key)   { return(Put(*o,key)); }
  boolean Put (Comparable *o)                { return(Put(*o,o)); }

  // the following put is some crazy attempt to recycle a "used" hashernode
  boolean Put ( HasherNode* n);         // returns TRUE if successful

  void ForcePut (Object &o, Comparable *key);       // jam it!
  void ForcePut (Object *o, Comparable *key) { ForcePut(*o,key); }
  void ForcePut (Comparable *o)              { ForcePut(*o,o); }

  // new data for first matching key; FALSE otherwise
  boolean Update (Object* o, Comparable* key);      
  boolean Update (Object& o, Comparable* key) { return Update(&o,key); }

  Object* Get (Comparable *key); 
  Object* Get (Comparable &key)             { return Get( &key); }
  boolean Get (Object **o, Comparable *key) { return 0 != ( *o=Get( key));}  // returns TRUE if in tree

  boolean Remove( Comparable *key);
  void    Rehash();
  void    Print ( ostream& out = cout) ;       // print the contents
  void    ShortPrint ( ostream& out = cout);  // just stats
  integer isa() { return HASHER_TREE; }
};

inline ostream& operator << (ostream &out, HasherTree& f)
{ f.Print(out); return out; }

inline ostream& operator << (ostream &out, HasherTree* f)
{ out << "(ptr->)"; f->Print(out); return out; }

////////////////////////////////////////////////////////////////////////////

class Hasher_iterator : public Object {
  
  List        stack;
  boolean     done;
  HasherNode* n;
public:
  Hasher_iterator(HasherTree& h, boolean remove = FALSE);
  Hasher_iterator(HasherTree* h, boolean remove = FALSE );
 ~Hasher_iterator() {;}

  HasherNode* GetNode()         { return n; }
  void          KillNode()     { delete n; }
  boolean     Update( Object* o) { if (n!=NULL) n->data = o; return( n!=NULL); }
  void        operator ++(); 
  void          operator --();     // incrementor for Remove macro
  void          FindBottom();     // find a node with null pointers, "push"ing nodes on the way
  boolean     end()          {return done;}
  Comparable* item(Object **d)      {*d = n->data; return n->key; }

};

////////////////////////////////////////////////////////////////////////////

class Hasher_updater : public Object {
  
  HasherTree *ht;
  HasherNode *n;
public:
  Hasher_updater(HasherTree& h)  { ht=&h; n=NULL;}
  Hasher_updater(HasherTree* h)  { ht= h; n=NULL;}
 ~Hasher_updater() {;}

  Object*  Get( Comparable* key);
  boolean  Update( Object* o)     { if (n!=NULL) n->data = o; return( n!=NULL); }
};

////////////////////////////////////////////////////////////////////////////

#define Hasher_ForEach_Object(ht, k, o)  \
        { \
           Object *o; \
           Comparable *k; \
           for ( Hasher_iterator hash__itr(ht); !hash__itr.end(); ++hash__itr) { \
             k = (Comparable *) hash__itr.item(&o);

#define Hasher_ForEach_Type(ht, t, k, o)  \
        { \
           t *o; \
           Comparable *k; \
           for ( Hasher_iterator hash__itr(ht); !hash__itr.end(); ++hash__itr) { \
             k = (Comparable *) hash__itr.item((Object **)&o);

#define Hasher_ForEach_Type_Remove(ht, t, k, o, n)  \
    { \
       t *o; \
       Comparable *k; \
       Hasher_iterator hash__itr(ht, TRUE ); \
       hash__itr.FindBottom(); \
       HasherNode *n; \
       for( ; !hash__itr.end() ; --hash_itr ) { \
         k = (Comparable *) hash_itr.item((Object **)&o); \
         n = hash__itr.GetNode();


#define Hasher_End_ForEach }}

////////////////////////////////////////////////////////////////////////////

#endif // HASHER_H

