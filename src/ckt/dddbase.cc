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
 *    $RCSfile: dddbase.cc,v $
 *    $Revision: 1.6 $
 *    $Date: 2002/11/21 02:40:15 $
 */

#include <iostream.h>
#include "ddd_base.h"
#include <stdlib.h>

// this line doesn't make much sense, since srand return void, so
// I am executing srand in DDDmanager init... --ALC
//  integer ZRand = srand((integer) time((time_t*) NULL));
integer ZRand;

/////////////////////////////////////////////////////////////
//
//  DDDnode structure : ( index, right, left )
//                       root
//                        /\
//                       /  \
//                     Left  Right
//                   True()  False()
//    Michael Mo
//    Modified by: F. Brewer, I. Radivojevic, C. Monahan
//    Copyright Regents of the University of California, 1994
/////////////////////////////////////////////////////////////

void
DDDnode::Print(ostream& out) {


	out << "DDDnode " << (void *) this 
		<< " v=" << index << " ref=" << NumReferences() << "\n";
    
	if(Then() && Else())
	{
			out << "  true=" << Then()->index<<", "<<(void *) Then() 
		<< " false=" << Else()->index<<", "<<(void *) Else()  << "\n";
	}
	else
	{
		out << "  true=" << (void *) Then() 
			<< " false=" << (void *) Else()  
			<< " " <<(dc?"ONE":"ZERO") << "\n";
	}


}

DDDmanager::DDDmanager(integer ) {

	one  = new DDDnode(-1, 0, 0, TRUE);  //if both edges point to one.. dc
	zero = new DDDnode(-1, 0, 0, FALSE); //if both edges point to zero.. not dc
	num_inputs = 0;
	num_nodes  = 2;
	num_cubes  = 0;
	unique     = new HasherTree*[MAXINDEX+1];
	computed   = new HashCache(DDD_PRIME);
	srand((integer) time((time_t*) NULL));
	ZRand = rand();
	label2elem = NULL;

}

DDDmanager::~DDDmanager() {
	delete one;
	delete zero;
	for( int i=0 ; i<num_inputs ; i++ )
    {
		unique[i]->SetDelete(TRUE,FALSE);    // delete the zbddnode during tree deletion
		delete unique[i];
    }
	delete [] unique;
	computed->Clear();
	delete computed;
}


DDDnode*
DDDmanager::GetNode( integer v, DDDnode *g, DDDnode *h ) {

	DDDnode *p;

	ASSERT( v >= 0, "Negative variable index!");

	ASSERT( v > g->index && v > h->index ,
			"GetNode index smaller than children's.");

	if( g == zero ) return h;   //Node elimination

	boolean    dc = ( g == h && h->isDC() && h->Index()+1 == v);
	DDDnode*  r = new DDDnode(v, g, h, dc);
	while( v>=num_inputs )
		unique[num_inputs++]=new HasherTree;

	if(  (p = (DDDnode*) unique[v]->Get( r)) ) {  //If already in the unique table
		delete r;
		return p;
	} else{
		unique[v]->Put(*r, r);                 //Input new node in unique table
		num_nodes++;                           //# of nodes increases
		Keep(g);
		Keep(h);
		return r;
	}
    
}


DDDnode*
DDDmanager::Subset1( DDDnode* P, integer var) {
  
	DDDcomputed2 *c;
	DDDnode* f;
	Object* o;

	ASSERT( var >= 0, "Negative variable index!");

	if(!P)
		return NULL;

	if( P->index < var ) return zero;

	if( P->index == var ) 
		return P->Then();
	else {
		c = new DDDcomputed2( SUBSET1, P, var );
		if( computed->Get( &o, c )) {
			delete c;
			return (DDDnode*) o;
		}
		else
			f =  GetNode( P->Index(), 
						  Subset1(P->Then(), var),
						  Subset1(P->Else(), var));
	}
	computed->Put( f, c );
	return f;
}


DDDnode*
DDDmanager::Subset0( DDDnode* P, integer var) {
  
	DDDcomputed2 *c;
	DDDnode* f;
	Object* o;

	ASSERT( var >= 0, "Negative variable index!");

	if(!P)
		return NULL;

	if( P->index < var ) return P;

	if( P->index == var ) 
		return ( P->Else() );
	else {
		c = new DDDcomputed2( SUBSET0, P, var );
		if( computed->Get( &o, c )) {
			delete c;
			return (DDDnode*) o;
		}
		else
			f = GetNode( P->Index(), 
						 Subset0( P->Then(), var),
						 Subset0( P->Else(), var));
	}
	computed->Put( f, c );
	return f;
}
 
DDDnode*
DDDmanager::Change( DDDnode* P, integer var) {

	DDDcomputed2 *c;
	DDDnode* f;
	Object* o;

	ASSERT( var >= 0, "Negative variable index!");

	c = new DDDcomputed2( CHANGE, P, var);
	if( computed->Get( &o, c )) {
		delete c;
		return (DDDnode*) o;
	}


	if( P->index < var )
		f = GetNode( var, P, zero);
	else if ( P->index == var ) 
		f = GetNode( var, 
					 P->Else(), 
					 P->Then());
	else 
		f = GetNode( P->Index(),
					 Change(P->Then(), var),
					 Change(P->Else(), var));

	computed->Put( f, c );
	return f;
}


DDDnode*
DDDmanager::Union( DDDnode* P, DDDnode* Q) {
  
	DDDcomputed1 *c;
	DDDnode* f;
	Object* o;

	if( P == zero ) return Q;
	if( Q == zero ) return P;
	if( P == Q ) return P;
  
	if( P->isDC() && P->Index() >= Q->Index()) return P;
	if( Q->isDC() && Q->Index() >= P->Index()) return Q;

	c = new DDDcomputed1( UNION, P, Q);
	if( computed->Get( &o, c )) {
		delete c;
		return (DDDnode*) o;
	}
	if( P->index > Q->index )
		f = GetNode( P->index, P->Then(), Union( P->Else(), Q) );
	else if ( P->index < Q->index )
		f = GetNode( Q->index, Q->Then(), Union( P, Q->Else()) );
	else
		f =  GetNode( P->Index(),
					  Union(P->Then(), Q->Then()),
					  Union(P->Else(), Q->Else()));

	computed->Put( f, c );
	return f;

}

DDDnode*
DDDmanager::Product( DDDnode* P, DDDnode* Q) {
  
	DDDcomputed1 *c;
	DDDnode* f;
	Object* o;

	//ASSERT( P->index != Q->index, "Invalid DDD for product");

	if( P == zero || Q == zero ) return zero;

	if( P == one )
		return Q;
	if( Q == one )
		return P;
  
	c = new DDDcomputed1( PRODUCT, P, Q);
	if( computed->Get( &o, c )) {
		delete c;
		return (DDDnode*) o;
	}

	if( P->index > Q->index )
		f = GetNode( P->index, Product(P->Then(), Q), Product(P->Else(), Q) );
	else if ( P->index < Q->index )
		f = GetNode( Q->index, Product(Q->Then(), P), Product(Q->Else(), P) );

	computed->Put( f, c );
	return f;

}

DDDnode*
DDDmanager::Intsect( DDDnode* P, DDDnode* Q ) {

	DDDcomputed1 *c;
	DDDnode* f;
	Object* o;

	if( P == zero ) return zero;
	if( Q == zero ) return zero;
	if( P == Q ) return P;
  
	if( P->isDC() && P->Index() >= Q->Index()) return Q;
	if( Q->isDC() && Q->Index() >= P->Index()) return P;

	c = new DDDcomputed1( INTSECT, P, Q);
	if( computed->Get( &o, c )) {
		delete c;
		return (DDDnode*) o;
	}

	if ( P->index < Q->index )
		f = Intsect( P, Q->Else() );
	else if( P->index == Q->index )
		f = GetNode( P->Index(),
					 Intsect(P->Then(), Q->Then()),
					 Intsect(P->Else(), Q->Else()));
	else 
		f = Intsect( P->Else(), Q );

	computed->Put( f, c );
	return f;
}


DDDnode*
DDDmanager::Diff( DDDnode* P, DDDnode* Q ) {

	DDDcomputed1 *c;
	DDDnode* f;
	Object* o;

	if( P == zero ) return zero;
	if( Q == zero ) return P;
	if( P == Q ) return zero;

	c = new DDDcomputed1( DIFF, P, Q);
	if( computed->Get( &o, c )) {
		delete c;
		return (DDDnode*) o;
	}
	if( P->index > Q->index )
		f = GetNode(P->Index(), 
					P->Then(),
					Diff(P->Else(), Q));
	else if( P->index < Q->index )
		f =  Diff( P, Q->Else());
	else 
		f = GetNode(P->Index(),
					Diff(P->Then(), Q->Then()),
					Diff(P->Else(), Q->Else()));

	computed->Put( f, c );
	return f;
}


integer
DDDnode::Compare(Comparable *p) {
 
	ASSERT( p->isa() == DDDNODE, "not a DDDnode Object");
 
	DDDnode *n = (DDDnode *)p;
 
	if (( index == n->index) && (left == n->left) &&
		( right == n->right))
		return 0;
	if ((integer)this > (integer)n)
		return 1;
	if ((integer)this < (integer)n)
		return -1;
	return 0;
}


//
// NodesInFunction, PathsInFunction (i.e. Count)...
//

integer
DDDmanager::NodesInFunction(DDDnode* f) {

	integer n;
	HasherTree *ht    = new HasherTree(FALSE);
	NodesInFunctionR(f, ht);
	n = ht->Count();
	delete ht;
	return n;
}

void
DDDmanager::NodesInFunctionR(DDDnode *f, HasherTree *ht) {
  
	if (f == one || f == zero)
		return;
	if (ht->Put(*f,f)) {
		if (f->Then())
			NodesInFunctionR(f->Then(), ht);
		if (f->Else())
			NodesInFunctionR(f->Else(), ht);
	} else
		return;
}

float
DDDmanager::Count( DDDnode* P ) {
	HasherTree *ht    = new HasherTree(TRUE);
	float       count = CountR( P, ht );

	delete ht;
	return count;
}

float
DDDmanager::CountR( DDDnode* P, HasherTree* ht) {
	Object *o;

	if (P == zero ) 
		return 0;
	if (P == one ) 
		return 1;
	if (ht->Get(&o, P))
		return ((FloatObj*)o)->val;

	float n;
	n = CountR(P->Then(), ht) +
		CountR(P->Else(), ht);
    
	ht->Put(new FloatObj(n), P);
	return n;
}

void
DDDmanager::PrintSets( DDDnode* P, ostream& out) {

	int i;
  
	ASSERT( P, "No Zbdd to print!" );

	out << "{\n";
	if ( P == one )
		out << "Base\n";
	else if ( P == zero )
		out << "Empty\n";
	else {
		cstring set = new char[num_inputs + 1];
		for (i = 0; i < num_inputs ; ++i)
			set[i] = '0';
		set[i] = '\0';
		PrintSetsR( P, set, out );
		delete [] set;
	}
	out << "}\n";
}

void
DDDmanager::PrintSetsR( DDDnode* P, cstring set, ostream& out) {
  
	if( P == one ) {
		out << set << "\n";
		num_cubes++;
		return;
	} else 
		if ( P == zero ) return;
		else {
			char c;
			c = set[ P->index ];
			set[ P->index ] = '1';
			PrintSetsR( P->Then(), set, out);
			set[ P->index ] = '0';
			PrintSetsR( P->Else(), set, out);
			set[ P->index ] = c;
		}
}
 
///////////////////////////////////////////////////////////////////////
//
// Universe routine generates universal set vith variables from
// [0, universe_top]
//

DDDnode*
DDDmanager::Universe(integer universe_top) {

	DDDnode*    temp_node = GetNode(0,one,one);
	for(integer i=1; i<=universe_top; i++)
		temp_node = GetNode(i,temp_node,temp_node);
	return temp_node;
}


void
DDDmanager::PrintNodes( DDDnode* P, ostream& out ) {

	if( P == zero  || P == one ){
		out << (void*)P <<"\n";
		exit(1);
	}  else {
		P->Print();
		if( P->left != one )
			PrintNodes( P->Then() );
		if( P->right != one && P->right != zero ) 
			PrintNodes( P->Else() );
	}
}

void
DDDmanager::TermPrint(ostream& out) {

	out << "Terminal Nodes: \n"
		<< "one = " << (void *) one << "\t"
		<< "zero = " << (void *) zero << "\n";
}


void
DDDmanager::PrintStats(ostream& out) {
/*
  out << "\n\nUNIQUE TABLE:\n\n";
  unique->ShortPrint(out);
//  unique->Print(out);
*/
	out << "\n\nCOMPUTED CACHE:\n";
	computed->Print(out);
}

///////////////////////////////////////////////////////////////////          
//
//  routine to perform garbage collection - DDDnodes supporting only
//  'unneeded' functions are deleted.
//
//
 
 
void
DDDmanager::GC() {

	ZRand = (integer) lrand48();
	for(integer i=(num_inputs-1); i>=0; i--)
		if(unique[i]->Count()!=0) 
			doGC(i);
	computed->Clear();
}
 
void
DDDmanager::ForceGC() {

	ZRand = (integer) lrand48();
	for(integer i=(num_inputs-1); i>=0; i--) 
		if(unique[i]->Count()!=0)
			doGC(i);
	computed->Clear();
}
 
void
DDDmanager::doGC(integer i) {

//  ZRand = (integer) lrand48();  
	DDDnode *node;
 
	HasherTree *unique_save = new HasherTree;
 
	Hasher_ForEach_Object(*unique[i], key, object) {
		node = (DDDnode *) key;
		if (node->NumReferences() == 0 && ! node->Marked())
			RGC(node);
	} Hasher_End_ForEach;
 
	Hasher_ForEach_Object(*unique[i], key, object) {
		node = (DDDnode *) key;
		if (node->NumReferences() > 0) {
			unique_save->Put(*node, node);
			node->UnMark();
#ifdef __USE_THIRD_TERM_GENERATION
			if(node->parents_stack)
			{
				node->parents_stack->ClearStack();
			}
#endif
				
		} else {
			ASSERT(node->Marked(), "deleting an unmarked node");
			delete node;
			--num_nodes;
		}
	} Hasher_End_ForEach;
 
	delete unique[i];
	unique[i] = unique_save;
//  computed->Clear();
}

void
DDDmanager::RGC(DDDnode *i) {
 
	if (i == one || i == zero)
		return;
 
	integer a, b;
	i->Mark();
 
	DDDnode *t = (DDDnode*) i->Then();
	DDDnode *e = (DDDnode*) i->Else();
 
	a = t->NumReferences();
	t->UnReference();
	if (a == 1)
		RGC(t);
 
	b = e->NumReferences();
	e->UnReference();
	if (b == 1)
		RGC(e);
}

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//    DDDcomputed functions
//


integer
DDDcomputed1::Compare(Comparable *p) {

	if (p->isa() == DDDCOMPUTED2) return -1;
	if (p->isa() == DDDCOMPUTED3) return -1;
	ASSERT( p->isa() == DDDCOMPUTED1, "not a BDDcomputed1 Object");
  
	DDDcomputed1 *n = (DDDcomputed1 *)p;
  
	if ((f == n->f) && (g == n->g) && ( type == n->type))
		return 0;
	if ((integer)this > (integer)n)
		return 1;
	if ((integer)this < (integer)n)
		return -1;
	return 0;
}

////////////////////////////////////////////////////////

integer
DDDcomputed2::Compare(Comparable *p) {

	if (p->isa() == DDDCOMPUTED1) return 1;
	if (p->isa() == DDDCOMPUTED3) return -1;
	ASSERT( p->isa() == DDDCOMPUTED2, "not a BDDcomputed2 Object");
  
	DDDcomputed2 *n = (DDDcomputed2 *)p;
  
	if ((f == n->f) && (type == n->type) && (var == n->var))
		return 0;
	if ((integer)this > (integer)n)
		return 1;
	if ((integer)this < (integer)n)
		return -1;
	return 0;
}

//////////////////////////////////////////////
integer
DDDcomputed3::Compare(Comparable *p) {

	if (p->isa() == DDDCOMPUTED1) return 1;
	if (p->isa() == DDDCOMPUTED2) return 1;

	ASSERT( p->isa() == DDDCOMPUTED3, "not a BDDcomputed3 Object");
  
	DDDcomputed3 *n = (DDDcomputed3 *)p;
  
	if ((f == n->f) && (g == n->g) && ( type == n->type))
		return 0;
	if ((integer)this > (integer)n)
		return 1;
	if ((integer)this < (integer)n)
		return -1;
	return 0;
}

//////////////////////////////////////////////

DDDnode*
DDDmanager::GetMinor( DDDnode* row, DDDnode* col) {
  
	DDDcomputed3 *c;
	Object* o;

	c = new DDDcomputed3( MINOR, row, col);
	if( computed->Get( &o, c )) {
		delete c;
		//cout<<"+";
		return (DDDnode*) o;
	}
	//cout<<"-";
	delete c;
	return (DDDnode *)NULL;
}

void
DDDmanager::PutMinor(DDDnode* minor, DDDnode* row, DDDnode* col){

	DDDcomputed3 *c, *cc;

	c = new DDDcomputed3( MINOR, row, col);
	cc = (DDDcomputed3 *)computed->Replace((Object *)minor, c );
	if(cc)
		delete cc;
	//computed->Put((Object *)minor, c );
}

void
DDDmanager::statistic(DDDnode *_ddd, ostream & out)
{
    out << "STATISTIC INFO FOR THE DDD"<<endl;
    out << "#nodes (DDD): " << NodesInFunction(_ddd) << endl; 
    out << "#paths (DDD): " << PathsInFunction(_ddd) << endl; 
    /*
	  out << "#total nodes (DDD): " << NumNodes() << endl;
	  out << "#total paths (DDD): " << NumCubes() << endl; 
	  out << "#total inputs (DDD): " << NumInputs() << endl; 
	  out << "#total node struture"<<endl;
    */
    //PrintSets(_ddd);
    /*
	  PrintNodes(_ddd);
	  cout << "#the unique/computed stats..."<<endl;
    */
    //PrintStats();
    out.flush();
}

///////////////////////////////////////////////////////////////////
//
//    Print a DDD tree
//

void 
DDDmanager::PrintDDDTree( DDDnode *_ddd )
{
	HasherTree *ht    = new HasherTree(FALSE);
	PrintDDDTreeR(_ddd, ht);
	delete ht;
}

void 
DDDmanager::PrintDDDTreeR( DDDnode *P, HasherTree *ht )
{
	if( P == zero  || P == one ){
		cout << (void*)P <<"\n";
		//exit(1);
	}  else {
		if (ht->Put(*P,P)) {
			if( P->left != one )
				PrintDDDTreeR( P->Then(), ht );
			if( P->right != one && P->right != zero ) 
				PrintDDDTreeR( P->Else(), ht );
			P->Print();
		}
	}
}
