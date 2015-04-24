/*
*******************************************************

        Symbolic Circuit Analysis With DDDs
                (*** SCAD3 ***)

   Xiang-Dong (Sheldon) Tan, 2002 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 5/14/2001 -> %G%
*******************************************************
*/

/*
 *    $RCSfile: new_term_gen4.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/11/21 02:40:15 $
 */


/* new_term_gen4.cc 
 *
 * Use a new dominant term generation algorithm based on 
 * shortest path algorithm. 

 * The main idea is to only update newly generated DDD vertices
 * instead of relaxing all the vertices we did before. The method is
 * different from the method in new_term_gen2.cc. In this method, the
 * shortest path is found from one vertex to each root vertex (reverse
 * graph of a DDD graph). In this way, we only need to upate newly
 * generated vertices each time without worrying about all the
 * existing vertices. So, the new algorithm should be O(n) after the
 * first terms is genreated (need formal proof).
 *
 *       X.-D. Sheldon Tan  June. 14, 2002
 * */

/* INCLUDES: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "globals.h"
#include "ddd_base.h"
#include "control.h"
#include "node.h"
#include "branch.h"
#include "complex.h"
#include "rclcompx.h"
#include "mna_form.h"
#include "mxexp.h"
#include "dtstruct.h"
#include "approx.h"
#include "acan.h"

//#define _DEBUG

#ifdef __USE_FOURTH_TERM_GENERATION


/*  Second shortest path term generation algortithm. It assume that
**  the first dominant term has been found prior. Find the shortest
**  path from root s to the 1-terminal in the given DDD.  It returns
**  the found path in DDD form.  It is used for fully expanded
**  coefficient DDD. Also note that once the path is found, we remove
**  it from s, so s will be changed.
**/

void
DDDmanager::FCRelax_4
(
	DDDnode *u, 
	DDDnode *v
)
{

    double val;
	double V = 0.0;
	rcl_complex cval;

    if(!u || !v)
	{
        return;
	}
    	
	if(v->Then() == u)
	{ // 1-edge
			
		DummBranch *dbch =  theFLabMap->get_dummbranch(v->index);
		assert(dbch);
		V = dbch->get_bvalue();	

		val = 0 - log10(fabs(V));
  
#ifdef _DEBUG
		cout <<"try to relax v: " 
			 << v->index << " " << (void *)v 
			 << "-> u: " << u->index 
			 << " " << (void *)u << endl;
		cout <<"v.val: " << real(v->val_ptr[0]) << " u+val: " 
			 << real(u->val_ptr[0]) + val << endl;
#endif
        if( real(v->val_ptr[0]) > (real(u->val_ptr[0]) + val) )
		{
            v->val_ptr[0] = real(u->val_ptr[0]) + val;
            v->pi = u;
#ifdef _DEBUG
			cout <<"v: " << v->index << "-> u: " << u->index << endl;
#endif
        }		

    }
    else 
	{ // 0-edge
        // 0.0 for 0-edge
#ifdef _DEBUG
		cout <<"try to relax v: " 
			 << v->index << " " 
			 << (void *)v << "-> u: " 
			 << u->index << " " << (void *)u << endl;
		cout <<"v.val: " << real(v->val_ptr[0]) << " u: " 
			 << real(u->val_ptr[0]) << endl;
#endif

        if( real(v->val_ptr[0]) > real(u->val_ptr[0]) )
		{
            v->val_ptr[0] = real(u->val_ptr[0]);
            v->pi = u;
#ifdef _DEBUG
			cout <<"v: " << v->index << "-> u: " << u->index << endl;
#endif
        }
    }
}


DDDnode *
DDDmanager::ExtractPath_4
(
	DDDnode *s
)
{
    DDDnode *path = Base();
    DDDnode *ddd_aux = s;
    if(!s)
	{
        return NULL;
	}

    for(;ddd_aux != one;ddd_aux = ddd_aux->pi)
	{
        if(ddd_aux->pi == ddd_aux->Then() ) //check it is 1-edge
		{
            path = Change(path,ddd_aux->index);
		}
    }

    return path;
}

/*
** get the next shortest path
*/

DDDnode *
DDDmanager::FCDDDNextShortestPath_4
(
	DDDnode **s
)
{	
	assert(s);

    if(*s == zero)
	{
        return zero;
	}
#ifdef _DEBUG    
    cout <<"Extract a new path ... " << endl;
#endif

	DDDnode *path = ExtractPath_4(*s);
	
	//fcoeff_print_symbolic_path(path,cout);

	if(path && path != one)
	{
		*s = DiffAndRelax(*s, path);
	}

	return path;
}

/*
** New Diff operation which build a new partial topology list
** for the operation of removing the given 
**/
DDDnode*
DDDmanager::DiffAndRelax
(
	DDDnode* P, 
	DDDnode* Q
) 
{
	DDDcomputed1 *c;
	DDDnode* f;
	Object* o;

	if( P == zero )
	{
		return zero;
	}
	if( Q == zero )
	{
		return P;
	}
	if( P == Q )
	{
		return zero;
	}

	c = new DDDcomputed1( DIFF, P, Q);
	
	if( computed->Get( &o, c )) 
	{
		delete c;
		return (DDDnode*) o;
	}

	if( P->index > Q->index )
	{
		f = GetNode
			(
				P->Index(), 
				P->Then(),
				DiffAndRelax(P->Else(), Q)
			);	
	}
	else if( P->index < Q->index )
	{
		f =  DiffAndRelax( P, Q->Else());
	}
	else
	{
		f = GetNode
			(
				P->Index(),
				DiffAndRelax(P->Then(), Q->Then()),
				DiffAndRelax(P->Else(), Q->Else())
			);
	}

	computed->Put( f, c );
	
	if(f != P)
	{
		//cout <<"relaxing new D: " << f << endl;
		if(!f->val_ptr)
		{
			f->val_ptr = new complex(0.0);
		}
		f->val_ptr[0] = INF_WEIGHT;
			
		// relax the newly created f vertex
		FCRelax_4(f->Then(), f);
		FCRelax_4(f->Else(), f);			
	}

	return f;
}


DDDnode *
DDDmanager::FCDDDShortestPath_4
(
	DDDnode *s
)
{
    DDDnode *ddd_aux;
    if(s == zero)
	{
        return zero;
	}	

	DDDqueue * topoList = new DDDqueue;

	DFS_ISS_4(s, topoList); // topological sorting and source initialization.
    //s->val_ptr[0] = 0.0;
    one->val_ptr[0] = 0.0;
	if(!topoList->isEmpty())
	{ // we have result
        //printf("stack size: %d\n", theTopoList->StackSize());
        while((ddd_aux = topoList->Dequque()) != NULL)
		{
            //printf("node:%d\t",ddd_aux->index);
			//cout <<"Relax node" << ddd_aux << endl;
            FCRelax_4(ddd_aux->Then(), ddd_aux);
            FCRelax_4(ddd_aux->Else(), ddd_aux);
        }
    }

    DFS_Restore(s); // color all the DDDnodes to `white`.
    
	return ExtractPath_4(s);
}

void
DDDmanager::DFS_ISS_4
(
	DDDnode *s,
	DDDqueue *queue
)
{
    if(!s)
	{
        return;
	}

    s->color = dBLACK;
    if(!s->val_ptr)
	{
        s->val_ptr = new complex(0.0);
	}

    s->val_ptr[0] = INF_WEIGHT;
    s->pi = NULL;
	if((s == zero) || (s == one))
	{
        return;
	}
    if(s->Then()->color == dWHITE)
	{
        DFS_ISS_4(s->Then(), queue);
    }
    if(s->Else()->color == dWHITE)
	{
        DFS_ISS_4(s->Else(), queue);
    }
    queue->Enquque(s);
}



#endif
