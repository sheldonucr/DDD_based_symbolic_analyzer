/*
*******************************************************

        Symbolic Circuit Analysis With DDDs
                (*** SCAD3 ***)

   Xiang-Dong (Sheldon) Tan, 1998 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 10/26/1998 -> %G%
*******************************************************
*/

/*
 *    $RCSfile: hasher.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/06/15 20:56:33 $
 */



#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include "list.h"
#include "hasher.h"

HasherNode::HasherNode(integer hv, Comparable *k, Object *o) {   // HashNode constructor

  right   =  left  =  0;
  hashval =  hv;
  key     =  k;
  data    =  o;
}

HasherNode::HasherNode(HasherNode *in, boolean copy)
{
   hashval = in->hashval;
   key     = in->key;
   data    = copy ? in->data->Copy() : in->data;

   if (in->right != NULL) 
   {
      right = new HasherNode(in->right, copy);
   }  else 
   {
      right = NULL;
   }

   if (in->left != NULL) 
   {
      left = new HasherNode(in->left, copy);
   }  
   else 
   {
      left = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////
// HasherTree constructor

HasherTree::HasherTree( boolean del_data, boolean del_key) { 
  
  count =  0;
  root        =  0;
  flags.max_depth = 10000;
  flags.del_data = (del_data)? TRUE:FALSE;
  flags.del_key  = (del_key )? TRUE:FALSE;
}

HasherTree::HasherTree(HasherTree *tree, boolean copy) {

   count   = tree->count;
   flags   = tree->flags;
   if (tree->root)
     root = new HasherNode(tree->root, copy);
   else
     root = 0;
}
////////////////////////////////////////////////////////////////////////////
//
// This rehashes the whole tree, in  the case where the 
// hashvals of the key's have changed, like BDDnode after a reorder.
// DONT TRY TO CHANGE THIS until you understand the -- and ++
// operators.
//
// The important question is whether the new HasherTree will still
// look like a tree (not a linked list).

void
HasherTree::Rehash() 
{
  List l;
  Hasher_ForEach_Object( this, k, o ) 
	  {
		  l.Add(hash__itr.GetNode());
	  }Hasher_End_ForEach
		   root = NULL; count = 0;
  List_ForEach_Type_Remove(&l, HasherNode, hn ) 
	  {
		  Put(hn);
	  }List_End_ForEach
}

////////////////////////////////////////////////////////////////////////////

void
HasherTree::Destroy(HasherNode *p) {    // recursive destruction of tree 

  if (p->right != NULL) 
  {
     Destroy(p->right);
     p->right = NULL;
  }
  if (p->left != NULL) 
  {
     Destroy(p->left);
     p->left = NULL;
  }

  if ( DelData()) delete p->data;
  if ( DelKey ()) delete p->key;

  delete p;
}
////////////////////////////////////////////////////////////////////////////

boolean
HasherTree::Put(Object& o, Comparable *key)
{
  HasherNode *q = NULL, *p = root;
  integer hashval = key->Hash();
  HASHER_MOD(hashval);

  for (;;) 
  {
    if (p == NULL) 
	{ 
		p = new HasherNode(hashval, key, &o);         // build a new node
		if (root == NULL)
			root = p;
		else 
		{
			if (q->hashval > hashval)
				q->right = p;
			else
				q->left = p;
		}
		++count;
		return TRUE;
    } 
    else if (p->hashval == hashval) 
	{ 
		if ( key->Compare(p->key) == 0 ) 
			return FALSE;                    // key already in tree
    }
    q = p;
    
    if (p->hashval > hashval)  
		p=p->right;
    else
		p=p->left;
  }
}

////////////////////////////////////////////////////////////////////////////

boolean
HasherTree::Put(HasherNode* node)
{
  HasherNode *q = NULL, *p = root;
  Comparable *key = node->key;
  integer hashval = key->Hash();
  node->left = node->right = NULL;
  HASHER_MOD(hashval);

  for (;;) 
  {
    if (p == NULL) 
	{ 
      p = node;
      node->hashval = hashval;
      if (root == NULL)
		  root = p;
      else 
	  {
		  if (q->hashval > hashval)
			  q->right = p;
		  else
			  q->left = p;
      }
      ++count;
      return TRUE;
    } 
    else if (p->hashval == hashval) 
	{ 
		if ( key->Compare(p->key) == 0 ) 
			return FALSE;                    // key already in tree
    }
    q = p;
    
    if (p->hashval > hashval)  
      p=p->right;
    else
      p=p->left;
  }
}

/////////////////////////////////////////////////////////////////////////
void
HasherTree::ForcePut(Object& o, Comparable *key) 
{

  HasherNode *q = NULL, *p = root;
  integer hashval = key->Hash();
  HASHER_MOD(hashval);

  for (;;) 
  {
    if (p == NULL) 
	{ 
      p = new HasherNode(hashval, key, &o);         // build a new node
      if (root == NULL)
		  root = p;
      else 
	  {
		  if (q->hashval > hashval)
			  q->right = p;
		  else
			  q->left = p;
      }
      ++count;
      return;
    } 
    else if (p->hashval == hashval) 
	{ 
		if ( key->Compare(p->key) == 0) 
		{
			
			if( DelData() ) delete p->data;
			p->data = &o;                    // Jam it! 
			
			if( DelKey() ) delete key;    // keys should match anyway
			
			return;                         
		} 
    }
    q = p;
    if (p->hashval > hashval) 
      p=p->right;
    else
		p=p->left;
  }
}

////////////////////////////////////////////////////////////////////////////
// TRUE menas a successful update

boolean
HasherTree::Update(Object* o, Comparable* key)
{
  integer hashval = key->Hash();
  HASHER_MOD(hashval);
  HasherNode *p = root;

  for (;;) 
  {
	  if (p == NULL) 
		  return FALSE;
	  
    else if (p->hashval == hashval) 
	{   // hashval match
		if ( key->Compare( p->key) == 0)
		{   // match!
			p->data = o;
			return TRUE;
		}
	}
	  p = (p->hashval > hashval) ? p->right : p->left;
  }
}

////////////////////////////////////////////////////////////////////////////

Object*
HasherTree::Get(Comparable *key)
{
  integer hashval = key->Hash();
  HASHER_MOD(hashval);
  HasherNode *p = root;

  for (;;) 
  {
	  if (p == NULL) 
		  return 0;
	  
	  else if (p->hashval == hashval) 
	  {   // hashval match
		  if ( key->Compare( p->key) == 0)   // match!
			  return p->data;
	  }
	  p = (p->hashval > hashval) ? p->right : p->left;
  }
}

////////////////////////////////////////////////////////////////////////////
//
//      o               o      |        o               o
//     / \             / \     |       / \             / \
//    p   x0          q   x0   |      p   x0          q   x0
//   / \             / \       |     / \        =>   / \
//  x1 x2           x1  x2     |    q   x1          x2  x1
//   \               \         |   / \
//   ...        =>   ...       |  x2 NULL
//     \               \       |________________________________________
//      r               r      |         o              o
//     / \             / \     |        / \    =>      / \
//    x3  q           x3  x4   |       p   x0         x1  x0
//       / \                   |      / \
//      x4 NULL                     NULL x1
//
// We are removing node p in the picture above, and replacing it with the
// node in position q--the first node with no right pointer, which is just
// less than the node p.
// o, p, q, and r are as shown above. x#'s represent any tree from that point
// down (anything from a single node to a full tree)
//
boolean
HasherTree::Remove( Comparable* node)
  {
  integer hashval = node->Hash();
  HASHER_MOD(hashval);
  HasherNode *p = root;
  HasherNode *o = NULL;

  for (;;) 
  {
    if( p == NULL )
      return FALSE;

    else if (*p->GetKey() == *node) 
	{ // hashval match
		
      if( DelData() ) delete p->data;
      if( DelKey() ) delete p->key;
      // now search for a node with no "right" pointer to put in its place
      HasherNode *r = NULL;
      HasherNode *q= (HasherNode *)p->Left() ; // Moved outside for loop -- Hien
      for(  ;
			q != NULL && q->Right() ;
			r = q, q = (HasherNode *)q->Right() );
	  
      if( r )            // => q exists, as well
	  {
		  r->Right(q->Left());
		  q->Left(p->Left());
	  }
      if( q )
		  q->Right(p->Right());
      else                      // case 3, set, but ->Right() can still be NULL
		  q = (HasherNode *)p->Right();

      // Now q is the node which is replacing p, or is NULL, so fix the parent (o)
      if( o )
		  ( o->Hashval() > hashval ) ? o->Right(q) : o->Left(q);
      else
		  root = q;
      count--;
      delete p;
      return TRUE;
      }
    o = p;
    if (p->Hashval() > hashval)
      p=(HasherNode *)p->Right();
    else
      p=(HasherNode *)p->Left();
    } // end for

  }

////////////////////////////////////////////////////////////////////////////

void
HasherTree::ShortPrint( ostream& out)
{ 
  flags.max_depth = 0;
  if (root != NULL) 
  {
	  CheckLevel(root, 0);
  } 
  else
	  out << "HasherTree : empty\n";
  out << "HasherTree : " << Count() << " item(s), " << (flags.max_depth+1) << " maximum depth.\n";
}

void
HasherTree::CheckLevel(HasherNode *p, int depth)
{
  if(p->right == NULL && p->left == NULL)
    flags.max_depth = MAX( flags.max_depth, (unsigned int)depth);
  if (p->right != NULL)
    CheckLevel(p->right, depth+1);
  if (p->left != NULL)
    CheckLevel(p->left,  depth+1);
}

////////////////////////////////////////////////////////////////////////////
 
void
HasherTree::Print( ostream& out)
{
  if (root != NULL) 
  {
    out << "HasherTree : " << Count() << " item(s).\n";
    PrintNode(root, 0, out);
  } 
  else
    out << "HasherTree : empty\n";
}
////////////////////////////////////////////////////////////////////////////

void
HasherTree::PrintNode(HasherNode *p, int depth, ostream& out)
{
  out << "Depth=" << depth << " Key-> ";
  if (p->key)
    out << p->key;

  if (p->data) 
  {
	  if ((integer)p->data != (integer)p->key)  
	  { // often the key and data are same
		  out << "Data-> " << p->data;
	  }
  }

  if (p->right != NULL)
    PrintNode(p->right, depth+1, out);
  if (p->left != NULL)
    PrintNode(p->left,  depth+1, out);
}

////////////////////////////////////////////////////////////////////////////

Hasher_iterator::Hasher_iterator(HasherTree& h, boolean remove) { // constructor
  if (h.root) 
  {
	  n = h.root;
	  done = FALSE;
	  if( remove )
		  // we may want to update the h.Count as well!
		  h.root = NULL;
  } 
  else 
  {
	  done = TRUE;
  }
}

////////////////////////////////////////////////////////////////////////////

Hasher_iterator::Hasher_iterator(HasherTree* h, boolean remove) 
{ // constructor
  if (h->root) {
    n = h->root;
    done = FALSE;
    if( remove )
      // we may want to update the h->Count as well!
      h->root = NULL;
  } else {
    done = TRUE;
  }
}

////////////////////////////////////////////////////////////////////////////  

void
Hasher_iterator::operator ++ () 
{      // iterator to walk through hash tree 

  if (done == TRUE)
    return;
  
  if (n->left != NULL) 
  {  // try to go left
	  stack.Add(*n);        // save current node
	  n = n->left;
	  return;
  } 
  while (n->right == NULL) 
  {
	  if (stack.Count() == 0) 
	  {
		  done = TRUE;
		  return;
	  }
	  n = (HasherNode *) stack.Pop();
  }
  n = n->right;
  return;
}


void
Hasher_iterator::operator -- () 
{

  if( stack.Count() == 0 )
  {
	  done = TRUE;
	  return;
  }

  n = (HasherNode *) stack.Pop();
  if( n->left != NULL )
	  n->left = NULL;
  else
	  n->right = NULL;
  
  FindBottom();
}

void
Hasher_iterator::FindBottom()
{
	  while( n->left != NULL || n->right != NULL )
	  {
		  stack.Add(*n);
		  if( n->left )
			  n = n->left;
		  else
			  n = n->right;
	  }
}


////////////////////////////////////////////////////////////////////////////  

Object*
Hasher_updater::Get( Comparable* key) 
{
	integer hashval = key->Hash();
	HASHER_MOD(hashval);
	n = ht->root;
	
	for (;;) 
	{
		if (n == NULL) 
			return 0;
		
		else if (n->hashval == hashval) 
		{   // hashval match
			if ( key->Compare( n->key) == 0)   // match!
				return n->data;
		}
		n = (n->hashval > hashval) ? n->right : n->left;
	}
}
