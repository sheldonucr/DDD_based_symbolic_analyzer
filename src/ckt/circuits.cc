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
 *    $RCSfile: circuits.cc,v $
 *    $Revision: 1.8 $
 *    $Date: 2002/11/21 02:40:15 $
 */

 
#define _DEBUG


/* INCLUDES: */
#include <assert.h>
#include "circuits.h"
#include "acan.h"
#include "ckt_call.h"
#include "mxexp.h"
#include "cancel_flab_map.h"

extern int read_input(char *file_name);
extern int EnBranchElim;
extern int UseSimpExp;

Circuit::Circuit
( 
	char *cir_name 
)
{

    // initialize the all the variables
    if(cir_name)
	{
        spice_file = CopyStr(cir_name);
	}
    else
	{
        error_mesg(INT_ERROR,"No file name givan.");
        return;
    }
        
    title = NULL;
    rnlist = NULL;
    rblist = NULL;
    nsize = 0;
    bsize = 0;
    extra_var_num = 0;
    analysisMode = aAC; 
    exp_method = SUBSET; 
    largestRealNode = 0;

    sub_ckt = NULL;
    ckt_call = NULL;
    cur_call = NULL;

    cir_type = cLinear;
    cir_level = cktTop;

    // set the initial label to 1
    IndexMatrix::reset_label();

    ddd = new DDDmanager;
    matrix = new MNAMatrix(this, ddd);
    ac_mgr = new ACAnalysisManager(matrix, ddd);
}

// constructor for sub ckt;
Circuit::Circuit()
{

    // initialize the all the variables
    title = NULL;
    spice_file = NULL; 
        
    rnlist = NULL;
    rblist = NULL;
    nsize = 0;
    bsize = 0;
    extra_var_num = 0;
    analysisMode = aAC; 
    exp_method = SUBSET; 
    largestRealNode = 0;

    sub_ckt = NULL;
    ckt_call = NULL;
    cur_call = NULL;

    cir_type = cLinear;
    cir_level = cktSub;

    ddd = NULL;
    matrix = NULL;
    ac_mgr = NULL; 
}

Circuit::~Circuit()
{
    delete [] spice_file;
    delete [] title;

    if(sub_ckt)
	{
        SubCircuit *ckt_aux, *p_ckt;
		ckt_aux = p_ckt = sub_ckt;
		while(ckt_aux)
		{
			p_ckt = ckt_aux->next;
			delete ckt_aux;
			ckt_aux = p_ckt;
		}
    }

	if(ckt_call)
	{
        CktCall   *c_aux, *pckt_call;
		c_aux =  pckt_call = ckt_call;		
		while(c_aux)
		{
			pckt_call = c_aux->next;
			delete c_aux;
			c_aux = pckt_call;
		}
    }

    if(rnlist)         delete rnlist;
    if(rblist)         delete rblist;
    if(matrix)         delete matrix;
    if(ddd)            delete ddd;
    if(ac_mgr)         delete ac_mgr;
}

int
Circuit::read_spice_file
(
	char *filename
)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"No ckt structure created, abort.");
        return -1;
    }
    
	theCurCkt = (SubCircuit *)theCkt;
    
	return read_input(filename);
}

/*
**  Build up the MNA matrix for the given circuits.
**  Due to the precence of subckt, before we process the
**  MNA in higher hierarchical level, we need to setup
**  all the MNA in lower level circuit. So, MNA setup 
**  has to be performed in DFS order.

    (1) Setup the MNA without considering the presence of 
    subckt calling.

    (2) set up all the MNA matrix of subckts if they exist.

    (3) go through all the subckt callings to build the 
    symbolic expressions.
*/

void
Circuit::build_mna()
{
    Node *Nlist = rnlist;
    DummBranch *Blist;
    MNARow    *cur_row;

    if((!rnlist || !rblist) && !ckt_call )
	{
        print_error(FAT_ERROR,"No netlist is loaded!");
        return;
    }

    /* setp (1), we go through all the nodes in "rnlist" */
    for(; Nlist ; Nlist = Nlist->Next())
	{

        if(!Nlist->get_real_num()) // skip the ground node
		{
            continue;
		}
        
        cur_row = matrix->new_mna_row(Nlist);
        /* --------------------------------------------------- 
		   Note: we don't create pivot element explicitly and all
		   the element inform about pivot elements are included in 
		   MNARow's blist. 
		   -----------------------------------------------------*/

        /* we then go though all the incident branch of the node */
        if(Nlist->lbranch->get_num() > 0)
		{
            Blist = Nlist->lbranch->get_blist();
            for(; Blist; Blist = Blist->Next())
			{
                cur_row->process_branch(Blist->get_branch());
			}
        }
        /*
		  else if(Nlist->get_type() == nNorm){
		  sprintf(_buf,"Dangling node found: %s",Nlist->name);
		  error_mesg(INT_ERROR,_buf);
		  }
        */

    }

    /* (2) building the MNA matrices for all subckts */
    if(sub_ckt)
	{
        SubCircuit    *ckt_aux = sub_ckt;
        for(;ckt_aux; ckt_aux = ckt_aux->next)
		{
            sprintf(_buf,"\nBuilding MNA for subckt: %s", ckt_aux->name);
            print_mesg(_buf);
            ckt_aux->build_mna();
            sprintf(_buf,"\nend of build subckt: %s", ckt_aux->name);
            print_mesg(_buf);
        }
    }

    /* (3) go through all the subckt calling to build the
	   composite MNA element */
	if(ckt_call)
	{
		CktCall    *c_aux = ckt_call;
		for(;c_aux;c_aux = c_aux->next)
		{
			sprintf(_buf,"\nProcess subckt call %s", c_aux->call_name);
			print_mesg(_buf);
			matrix->process_subckt_call(c_aux);
		}
	}
	/*
	  else
	  print_warn("No subckt call statement exists.");
	*/
        
    matrix->calc_value();
    matrix->create_index();
    matrix->build_sys_ddd();
    matrix->print_mna();
}


/*
** functions for rnlist operations
*/

void
Circuit::add_node
(
	Node *node
)
{
    
    Node *prev_ptr, *node_ptr;

    /* first add to extra node real_num's so all
       extra nodes will have real_num > largestRealNode
    */
    if(node->real_num > largestRealNode && !node->isExtraVar )
	{
        for(node_ptr = rnlist; node_ptr; node_ptr = node_ptr->next)
		{
            if(node_ptr->isExtraVar)
                node_ptr->real_num += 
                        (node->real_num - largestRealNode);
        }
        largestRealNode = node->real_num;
    }

    if( node->isExtraVar)
	{
        node->real_num += largestRealNode; 
	}

    /* insert the node into the sorted node list */
    for(prev_ptr = NULL, node_ptr = rnlist ;
         node_ptr && node_ptr->num < node->num;
         prev_ptr = node_ptr, node_ptr = node_ptr->next) ;
    
    if(!node_ptr || node_ptr->num != node->num)
	{
        if(!rnlist)
		{
            rnlist = prev_ptr = node;
		}
        else if(!prev_ptr)
		{
            node->next = rnlist;
            rnlist = node;
        }
        else
		{
            prev_ptr->next = node;
            node->next = node_ptr;
        }
        nsize++;
    }
    else
	{
        error_mesg(INT_ERROR,"Node already exist. adding aborts");
	}
}

void
Circuit::print_nodes
(
	ostream & out
)
{
    Node *n_aux = rnlist;

    out << "Nodes in real node list(real_num): ";
    for(; n_aux; n_aux = n_aux->next)
	{
        out <<n_aux->get_real_num() <<" ";
    }
	out <<endl;
}


/* 
** find the Node sturture, given
** its read number.
*/

Node *
Circuit::query_node
(
	int node
)
{
    Node    *npt = rnlist;

    if(!rnlist)
	{
        return NULL;
	}

    for(;npt; npt = npt->next) 
	{
        if(npt->real_num == node)
		{
            return npt;
		}
    }

    return (Node *)NULL;
}

/* 
**    functions for rblist operation
*/
void
Circuit::add_branch
(
	Branch *_b
)
{
    Branch *b_aux = rblist;

    if(!_b)
	{
        return;
    }

    if(!b_aux)
	{
        rblist = _b;
	}
    else
	{
        while(b_aux->next) b_aux = b_aux->next;
        b_aux->next = _b;
    }

    bsize++;
}

/*----------------------------------------------------
Spec: find a branch given the branch name in the 
      global branch list
-----------------------------------------------------*/
Branch *
Circuit::query_branch
(
	const char *_name
)
{
    if(!_name)
	{
        return NULL;
	}

    Branch *b_aux = rblist;
    char *str = CopyStr(_name);
 
    for(; b_aux; b_aux = b_aux->next)
	{
        if(!strcmp(b_aux->name, ToLower(str)))
		{
            //delete [] str;
            return b_aux;
        }
    }
    delete [] str;
    return (Branch *)NULL;
}

/*----------------------------------------------------
Spec: check if the branch can be lumped into
       existing branches.
-----------------------------------------------------*/
Branch *
Circuit::check_lump
(
	Node *_node1, 
	Node *_node2, 
	DeviceType _type
)
{
    if(!_node1 || !_node2)
	{
        return NULL;
	}

    Branch *b_aux = rblist;
 
    for(; b_aux; b_aux = b_aux->next)
	{
		if(b_aux->get_stat() == B_DELETED)
			continue;
        if(b_aux->get_type() == _type)
		{
			if(((b_aux->node1->get_real_num() == _node1->get_real_num()) &&
				(b_aux->node2->get_real_num() == _node2->get_real_num())) ||
			   ((b_aux->node1->get_real_num() == _node2->get_real_num()) &&
				(b_aux->node2->get_real_num() == _node1->get_real_num())))
				{
					return b_aux;
				}
		}

    }

    return (Branch *)NULL;
}

/*
**    functions for sub circuit and subckt calling operations
*/

/*----------------------------------------------------
Spec: add a sub-circuit into a subckt list of current ckt.
The the order of the list will be the order of adding.
-----------------------------------------------------*/
void
Circuit::add_sub_ckt
(
	SubCircuit *_ckt
)
{
    SubCircuit    *ckt_aux = sub_ckt;
    if(!sub_ckt)
	{
        sub_ckt    = _ckt;
	}
    else 
	{
        while(ckt_aux->next)    ckt_aux = ckt_aux->next;
        ckt_aux->next = _ckt;
    }
}

/*----------------------------------------------------
Spec: add a circuit calling into corresponding list.
The the order of the list will be the order of adding.
the added subckt call also become current calling of
the circuit pointed by cur_call.
-----------------------------------------------------*/
void
Circuit::add_ckt_call
(
	CktCall *_call
)
{
    if(!_call)
	{
        return;
	}
    CktCall    *call_aux = ckt_call;
    if(!ckt_call)
	{
        ckt_call    = _call;
	}
    else 
	{
        while(call_aux->next)    call_aux = call_aux->next;
    
		call_aux->next = _call;
    }
    cur_call = _call;
}

/*
**  create a new DDD HasherTree for each subcircuit
*/
void
Circuit::sub_init_ht()
{
    SubCircuit *sckt;

    for(sckt = sub_ckt; sckt; sckt = sckt->next)
	{
        sckt->init_ht();
	}
}

/*
 * functions for remove cancelling terms (de-cancel)
 */
void
Circuit::fcoeff_decancel()
{
    Branch *blaux, *blaux1;
    MNAElement *elem;
    int    n1,n2,n3,n4;
    int    ind, ind1, ind2, ind3, ind4;
    if(!rblist)
	{
        return;
	}

    print_mesg("fcoeff post sDDD decancellation ...");

    /* we through all the branches */
    for(blaux = rblist; blaux; blaux = blaux->Next())
	{
        if(blaux->get_stat() == B_DELETED)
		{
            continue;
		}
     
		if(blaux->get_type() == dRES ||
           blaux->get_type() == dCAP ||
           blaux->get_type() == dIND )
		{

            /* first we skip branch with one point
			   connecting ground. */
            if(!blaux->get_node1()->get_real_num() ||
			   !blaux->get_node2()->get_real_num())
			{
                continue;
			}

            n1 = blaux->get_node1()->get_index();
            n2 = blaux->get_node2()->get_index();
#ifdef _DEBUG
            cout<<"Bname:"<<blaux->get_name()
				<<" n1:"<<n1
				<<" n2:"<<n2<<endl;
#endif
            /*       n1    n2 
               n1    ind1    ind2
               n2    ind3    ind4
            */
            ind = matrix->get_label(n1,n1);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind1 = elem->find_findex(blaux->get_name(),ind);
           

            ind = matrix->get_label(n1,n2);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind2 = elem->find_findex(blaux->get_name(),ind);
           

            ind = matrix->get_label(n2,n1);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind3 = elem->find_findex(blaux->get_name(),ind);
         

            ind = matrix->get_label(n2,n2);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind4 = elem->find_findex(blaux->get_name(),ind);
          

            ac_mgr->fcoeff_decancel(ind1, ind2, ind3, ind4);
        }
        else if( blaux->get_type() == dVCCS ) 
		{

            /*  we first consider generatal term cancelling case
                with following pattern which is generated by
				the branch and other branchs.
                a   -b
                -a   b
            */
            for(blaux1 = rblist; blaux1; blaux1= blaux1->Next())
			{
                if(blaux1 != blaux)
				{
                    fcoeff_gen_decancel(blaux, blaux1);
				}
            }

			/*
            for(blaux1 = blaux->Next(); blaux1; 
                    blaux1= blaux1->Next())
                fcoeff_gen_decancel(blaux,blaux1);
            */

			// then we consider the cancelling terms generated
			// by the branch itself.
			if(!blaux->get_node1()->get_real_num() ||
			   !blaux->get_node2()->get_real_num() ||
			   !blaux->get_node3()->get_real_num() ||
			   !blaux->get_node4()->get_real_num())
			{
				continue;
			}

			n1 = blaux->get_node1()->get_index();
            n2 = blaux->get_node2()->get_index();
            n3 = blaux->get_node3()->get_index();
            n4 = blaux->get_node4()->get_index();
#ifdef _DEBUG
            cout<<"Bname:"<<blaux->get_name()
				<<" n1:"<<n1
				<<" n1:"<<n2
				<<" n3:"<<n3
				<<" n4:"<<n4<<endl;
#endif

            /*        n3    n4 
                n1    ind1    ind2
                n2    ind3    ind4
            */
            ind = matrix->get_label(n1,n3);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind1 = elem->find_findex(blaux->get_name(),ind);

            ind = matrix->get_label(n1,n4);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind2 = elem->find_findex(blaux->get_name(),ind);

            ind = matrix->get_label(n2,n3);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind3 = elem->find_findex(blaux->get_name(),ind);

            ind = matrix->get_label(n2,n4);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind4 = elem->find_findex(blaux->get_name(),ind);

            ac_mgr->fcoeff_decancel(ind1, ind2, ind3, ind4);

        }
    }
    print_mesg("End of DDD decancellation");

	/* caluculate all numerical value coefficient */
	ac_mgr->calc_fcoeff_list_value();   
    ac_mgr->fcoeff_freq_sweep(FALSE);

}


void
Circuit::fcoeff_decancel_op
(
	int n1, int n2, 
	int n3, int n4,
    char *name1, char *name2, 
	char *name3, char *name4
)
{
    MNAElement *elem;
    int ind, ind1, ind2, ind3, ind4;

    /*        n3    n4 
        n1    ind1    ind2
        n2    ind3    ind4
    */
    ind = matrix->get_label(n1,n3);
    elem = ddd->get_label2elem()->get_elem(ind);
    ind1 = elem->find_findex(name1,ind);

    ind = matrix->get_label(n1,n4);
    elem = ddd->get_label2elem()->get_elem(ind);
    ind2 = elem->find_findex(name2,ind);

    ind = matrix->get_label(n2,n3);
    elem = ddd->get_label2elem()->get_elem(ind);
    ind3 = elem->find_findex(name3,ind);

    ind = matrix->get_label(n2,n4);
    elem = ddd->get_label2elem()->get_elem(ind);
    ind4 = elem->find_findex(name4,ind);

    ac_mgr->fcoeff_decancel(ind1, ind2, ind3, ind4);
}

/* perform general term cancelling elimination
** we assume branch1 is VCCS type.
*/

void
Circuit::fcoeff_gen_decancel
(
	Branch *branch1, 
	Branch *branch2
)
{
    /*        3    4 
        1    ind1    ind2
        2    ind3    ind4
    */
    int ind, num = 0;
    int b11,b12,b13, b14;
    int b21, b22, b23, b24;

    b11 = b12 = b13 = b14 = -1;
    b21 = b22 = b23 = b24 = -1;

#ifdef _DEBUG
    cout <<"A vccs branch with other branch ....";
    cout <<" branch: " << branch1->get_name()
		 <<" and branch: " << branch2->get_name()<<endl;
#endif

    if(branch1->get_node1()->get_real_num())
	{
        b11 = branch1->get_node1()->get_index();
        num++;
    }
    if(branch1->get_node2()->get_real_num())
	{
        b12 = branch1->get_node2()->get_index();
        num++;
    }
    if(branch1->get_node3()->get_real_num())
	{
        b13 = branch1->get_node3()->get_index();
        num++;
    }
    if(branch1->get_node4()->get_real_num())
	{
        b14 = branch1->get_node4()->get_index();
        num++;
    }

    /* at least three of them must be no ground node */
    //cout<<"num in step1:"<<num<<endl;
    //cout.flush();
    if(num < 3)
	{
        return;
	}

    // sort
    if( b11 < b12)
	{
        ind = b11; b11 = b12; b12 = ind;
    }

    if( b13 < b14)
	{
        ind = b13; b13 = b14; b14= ind;
    }

    //cout<<"step1"<<endl;
    //cout<<"b11: "<<b11<<" b12:"<<b12<<" b13: "<<b13<<" b14: "<<b14<<endl;
        
    num = 0;
    if(branch2->get_node1()->get_real_num())
	{
        b21 = branch2->get_node1()->get_index();
        num++;
    }
    if(branch2->get_node2()->get_real_num())
	{
        b22 = branch2->get_node2()->get_index();
        num++;
    }
    if( branch2->get_type() == dVCCS ) 
	{
        if(branch2->get_node3()->get_real_num())
		{
            b23 = branch2->get_node3()->get_index();
            num++;
        }
        if(branch2->get_node4()->get_real_num())
		{
            b24 = branch2->get_node4()->get_index();
            num++;
        }
    } 
	else 
	{
        b23 = b21; if(b23 != -1) num++;
        b24 = b22; if(b24 != -1) num++;
    }
        
    /* at least three of them must be no ground node */
    //cout<<"num in step2:"<<num<<endl;
    //cout.flush();
    if(num < 3)
	{
        return;
	}

    // sort
    if( b21 < b22)
	{
        ind = b21; b21 = b22; b22 = ind;
    }

    if( b23 < b24)
	{
        ind = b23; b23 = b24; b24= ind;
    }

    //cout<<"step2"<<endl;
    //cout<<"b21: "<<b21<<" b22:"<<b22<<" b23: "<<b23<<" b24: "<<b24<<endl;

    /*          3    4 
        1    ind1    ind2
        2    ind3    ind4
    */

    if( (b11 == b21) && (b12 == b22) &&
        (b11 != -1) && (b12 != -1) )
	{
        if((b13 != -1) && (b23 != -1) && (b13 != b23))
		{
            //cout<<"case11"<<endl;
            fcoeff_decancel_op(b11, b12, b13, b23,
							   branch1->get_name(),branch2->get_name(),
							   branch1->get_name(),branch2->get_name());
        }
        if((b13 != -1) && (b24 != -1) && (b13 != b24))
		{
            //cout<<"case12"<<endl;
            fcoeff_decancel_op(b11, b12, b13, b24,
							   branch1->get_name(),branch2->get_name(),
							   branch1->get_name(),branch2->get_name());
        }
        if((b14 != -1) && (b23 != -1) && (b14 != b23))
		{
            //cout<<"case13"<<endl;
            fcoeff_decancel_op(b11, b12, b14, b23,
							   branch1->get_name(),branch2->get_name(),
							   branch1->get_name(),branch2->get_name());
        }
        if((b14 != -1) && (b24 != -1) && (b14 != b24))
		{
            //cout<<"case14"<<endl;
            fcoeff_decancel_op(b11, b12, b14, b24,
							   branch1->get_name(),branch2->get_name(),
							   branch1->get_name(),branch2->get_name());
        }
    }

    if( (b13 == b23) && (b14 == b24) &&
        (b13 != -1) && (b14 != -1) ){
        if((b11 != -1) && (b21 != -1) && (b11 != b21))
		{
            //cout<<"case21"<<endl;
            fcoeff_decancel_op(b11, b21, b13, b14,
							   branch1->get_name(),branch1->get_name(),
							   branch2->get_name(),branch2->get_name());
        }
        if((b11 != -1) && (b22 != -1) && (b11 != b22))
		{
            //cout<<"case22"<<endl;
            fcoeff_decancel_op(b11, b22, b13, b14,
							   branch1->get_name(),branch1->get_name(),
							   branch2->get_name(),branch2->get_name());
        }
        if((b12 != -1) && (b21 != -1) && (b12 != b21))
		{
            //cout<<"case23"<<endl;
            fcoeff_decancel_op(b12, b21, b13, b14,
							   branch1->get_name(),branch1->get_name(),
							   branch2->get_name(),branch2->get_name());
        }
        if((b12 != -1) && (b22 != -1) && (b12 != b22))
		{
            //cout<<"case24"<<endl;
            fcoeff_decancel_op(b12, b22, b13, b14,
							   branch1->get_name(),branch1->get_name(),
							   branch2->get_name(),branch2->get_name());
        }
    }
}


/* functions for remove cancelling terms (de-cancel) The differences
 * between following functions and those above are that the
 * de-cancelling takes place during sDDD construction. So, the new
 * de-cancelling algorithm should be faster and more space-efficient
 * than the previous one.  */
void
Circuit::fcoeff_new_decancel()
{
    Branch *blaux, *blaux1;
    MNAElement *elem;
    int    n1,n2,n3,n4;
    int    ind, ind1, ind2, ind3, ind4;
    if(!rblist)
	{
        return;
	}

    print_mesg("new fcoeff sDDD decancellation map construction...");

	// build the cancelling terms map that tells
	// which two individual entries cancel each other.
	if(theCancelFLabMap)
	{
		delete theCancelFLabMap;
	}

	assert(matrix);
	int map_size = (matrix->num_elem + 1)*
		(matrix->max_branch_num() + 1);

	cout<<"size of de-cancel map list:"<< map_size << endl;

	theCancelFLabMap = new CancelFLabMap(map_size);

    /* we through all the branches */
    for(blaux = rblist; blaux; blaux = blaux->Next())
	{
        if(blaux->get_stat() == B_DELETED)
		{
            continue;
		}
     
		if(blaux->get_type() == dRES ||
           blaux->get_type() == dCAP ||
           blaux->get_type() == dIND )
		{

            /* first we skip branch with one point
			   connecting ground. */
            if(!blaux->get_node1()->get_real_num() ||
			   !blaux->get_node2()->get_real_num())
			{
                continue;
			}

            n1 = blaux->get_node1()->get_index();
            n2 = blaux->get_node2()->get_index();
#ifdef _DEBUG
            cout<<"Bname:"<<blaux->get_name()
				<<" n1:"<<n1
				<<" n2:"<<n2<<endl;
#endif
            /*       n1    n2 
               n1    ind1    ind2
               n2    ind3    ind4
            */
            ind = matrix->get_label(n1,n1);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind1 = elem->find_findex(blaux->get_name(),ind);
           

            ind = matrix->get_label(n1,n2);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind2 = elem->find_findex(blaux->get_name(),ind);
           
            ind = matrix->get_label(n2,n1);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind3 = elem->find_findex(blaux->get_name(),ind);
          

            ind = matrix->get_label(n2,n2);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind4 = elem->find_findex(blaux->get_name(),ind);
           

			theCancelFLabMap->add_cancel_lab_pair(ind1, ind4);
			theCancelFLabMap->add_cancel_lab_pair(ind2, ind3);
			
        }
        else if( blaux->get_type() == dVCCS ) 
		{

            /*  we first consider generatal term cancelling case
                with following pattern which is generated by
				the branch and other branchs.
                a   -b
                -a   b
            */
            for(blaux1 = rblist; blaux1; blaux1= blaux1->Next())
			{
                if(blaux1 != blaux)
				{
                    fcoeff_new_gen_decancel(blaux, blaux1);
				}
            }

			// then we consider the cancelling terms generated
			// by the branch itself.
			if(!blaux->get_node1()->get_real_num() ||
			   !blaux->get_node2()->get_real_num() ||
			   !blaux->get_node3()->get_real_num() ||
			   !blaux->get_node4()->get_real_num())
			{
				continue;
			}

			n1 = blaux->get_node1()->get_index();
            n2 = blaux->get_node2()->get_index();
            n3 = blaux->get_node3()->get_index();
            n4 = blaux->get_node4()->get_index();
#ifdef _DEBUG
            cout<<"Bname:"<<blaux->get_name()
				<<" n1:"<<n1
				<<" n1:"<<n2
				<<" n3:"<<n3
				<<" n4:"<<n4<<endl;
#endif

            /*        n3    n4 
                n1    ind1    ind2
                n2    ind3    ind4
            */
            ind = matrix->get_label(n1,n3);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind1 = elem->find_findex(blaux->get_name(),ind);

            ind = matrix->get_label(n1,n4);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind2 = elem->find_findex(blaux->get_name(),ind);

            ind = matrix->get_label(n2,n3);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind3 = elem->find_findex(blaux->get_name(),ind);

            ind = matrix->get_label(n2,n4);
            elem = ddd->get_label2elem()->get_elem(ind);
            ind4 = elem->find_findex(blaux->get_name(),ind);

			theCancelFLabMap->add_cancel_lab_pair(ind1, ind4);
			theCancelFLabMap->add_cancel_lab_pair(ind2, ind3);


        }
    }

	
    print_mesg("End of new sDDD decancellation map construction");

#ifdef _DEBUG
    theCancelFLabMap->print_cancel_lab_map();
#endif

}


void
Circuit::fcoeff_new_decancel_op
(
	int n1, int n2, 
	int n3, int n4,
    char *name1, char *name2, 
	char *name3, char *name4
)
{
    MNAElement *elem;
    int ind, ind1, ind2, ind3, ind4;

    /*        n3    n4 
        n1    ind1    ind2
        n2    ind3    ind4
    */
    ind = matrix->get_label(n1,n3);
    elem = ddd->get_label2elem()->get_elem(ind);
    ind1 = elem->find_findex(name1,ind);

    ind = matrix->get_label(n1,n4);
    elem = ddd->get_label2elem()->get_elem(ind);
    ind2 = elem->find_findex(name2,ind);

    ind = matrix->get_label(n2,n3);
    elem = ddd->get_label2elem()->get_elem(ind);
    ind3 = elem->find_findex(name3,ind);

    ind = matrix->get_label(n2,n4);
    elem = ddd->get_label2elem()->get_elem(ind);
    ind4 = elem->find_findex(name4,ind);

	theCancelFLabMap->add_cancel_lab_pair(ind1, ind4);
	theCancelFLabMap->add_cancel_lab_pair(ind2, ind3);

}

/* perform general term cancelling elimination
** we assume branch1 is VCCS type.
*/

void
Circuit::fcoeff_new_gen_decancel
(
	Branch *branch1, 
	Branch *branch2
)
{
    /*        3    4 
        1    ind1    ind2
        2    ind3    ind4
    */
    int ind, num = 0;
    int b11,b12,b13, b14;
    int b21, b22, b23, b24;

    b11 = b12 = b13 = b14 = -1;
    b21 = b22 = b23 = b24 = -1;

#ifdef _DEBUG
    cout <<"A vccs branch with other branch ....";
    cout <<" branch: " << branch1->get_name()
		 <<" and branch: " << branch2->get_name()<<endl;
#endif
    if(branch1->get_node1()->get_real_num())
	{
        b11 = branch1->get_node1()->get_index();
        num++;
    }
    if(branch1->get_node2()->get_real_num())
	{
        b12 = branch1->get_node2()->get_index();
        num++;
    }
    if(branch1->get_node3()->get_real_num())
	{
        b13 = branch1->get_node3()->get_index();
        num++;
    }
    if(branch1->get_node4()->get_real_num())
	{
        b14 = branch1->get_node4()->get_index();
        num++;
    }

    /* at least three of them must be no ground node */
    //cout<<"num in step1:"<<num<<endl;
    //cout.flush();
    if(num < 3)
	{
        return;
	}

    // sort
    if( b11 < b12)
	{
        ind = b11; b11 = b12; b12 = ind;
    }

    if( b13 < b14)
	{
        ind = b13; b13 = b14; b14= ind;
    }

    //cout<<"step1"<<endl;
    //cout<<"b11: "<<b11<<" b12:"<<b12<<" b13: "<<b13<<" b14: "<<b14<<endl;
        
    num = 0;
    if(branch2->get_node1()->get_real_num())
	{
        b21 = branch2->get_node1()->get_index();
        num++;
    }
    if(branch2->get_node2()->get_real_num())
	{
        b22 = branch2->get_node2()->get_index();
        num++;
    }
    if( branch2->get_type() == dVCCS ) 
	{
        if(branch2->get_node3()->get_real_num())
		{
            b23 = branch2->get_node3()->get_index();
            num++;
        }
        if(branch2->get_node4()->get_real_num())
		{
            b24 = branch2->get_node4()->get_index();
            num++;
        }
    } 
	else 
	{
        b23 = b21; if(b23 != -1) num++;
        b24 = b22; if(b24 != -1) num++;
    }
        
    /* at least three of them must be no ground node */
    //cout<<"num in step2:"<<num<<endl;
    //cout.flush();
    if(num < 3)
	{
        return;
	}

    // sort
    if( b21 < b22)
	{
        ind = b21; b21 = b22; b22 = ind;
    }

    if( b23 < b24)
	{
        ind = b23; b23 = b24; b24= ind;
    }

    //cout<<"step2"<<endl;
    //cout<<"b21: "<<b21<<" b22:"<<b22<<" b23: "<<b23<<" b24: "<<b24<<endl;

    /*          3    4 
        1    ind1    ind2
        2    ind3    ind4
    */

    if( (b11 == b21) && (b12 == b22) &&
        (b11 != -1) && (b12 != -1) )
	{
        if((b13 != -1) && (b23 != -1) && (b13 != b23))
		{
            //cout<<"case11"<<endl;
            fcoeff_new_decancel_op(b11, b12, b13, b23,
							   branch1->get_name(),branch2->get_name(),
							   branch1->get_name(),branch2->get_name());
        }
        if((b13 != -1) && (b24 != -1) && (b13 != b24))
		{
            //cout<<"case12"<<endl;
            fcoeff_new_decancel_op(b11, b12, b13, b24,
							   branch1->get_name(),branch2->get_name(),
							   branch1->get_name(),branch2->get_name());
        }
        if((b14 != -1) && (b23 != -1) && (b14 != b23))
		{
            //cout<<"case13"<<endl;
            fcoeff_new_decancel_op(b11, b12, b14, b23,
							   branch1->get_name(),branch2->get_name(),
							   branch1->get_name(),branch2->get_name());
        }
        if((b14 != -1) && (b24 != -1) && (b14 != b24))
		{
            //cout<<"case14"<<endl;
            fcoeff_new_decancel_op(b11, b12, b14, b24,
							   branch1->get_name(),branch2->get_name(),
							   branch1->get_name(),branch2->get_name());
        }
    }

    if( (b13 == b23) && (b14 == b24) &&
        (b13 != -1) && (b14 != -1) ){
        if((b11 != -1) && (b21 != -1) && (b11 != b21))
		{
            //cout<<"case21"<<endl;
            fcoeff_new_decancel_op(b11, b21, b13, b14,
							   branch1->get_name(),branch1->get_name(),
							   branch2->get_name(),branch2->get_name());
        }
        if((b11 != -1) && (b22 != -1) && (b11 != b22))
		{
            //cout<<"case22"<<endl;
            fcoeff_new_decancel_op(b11, b22, b13, b14,
							   branch1->get_name(),branch1->get_name(),
							   branch2->get_name(),branch2->get_name());
        }
        if((b12 != -1) && (b21 != -1) && (b12 != b21))
		{
            //cout<<"case23"<<endl;
            fcoeff_new_decancel_op(b12, b21, b13, b14,
							   branch1->get_name(),branch1->get_name(),
							   branch2->get_name(),branch2->get_name());
        }
        if((b12 != -1) && (b22 != -1) && (b12 != b22))
		{
            //cout<<"case24"<<endl;
            fcoeff_new_decancel_op(b12, b22, b13, b14,
							   branch1->get_name(),branch1->get_name(),
							   branch2->get_name(),branch2->get_name());
        }
    }
}


/*  Perform the eliminations of circuit elements in the complex
    DDDs. We will remove all the appearances of the element
    admittances with error limits.  We go through the Branch list of
    the mian circuit.  we assume the MNA matrix and index matrix have
    been constructed.

*/
void
Circuit::whole_elem_elim()
{
    Branch *blaux;
    int    n1,n2,n3,n4;
    int    ind1, ind2, ind3, ind4;
    if(!rblist)
	{
        return;
	}

    print_mesg("Simplification by whole element elimination ... ");


    /* we through all the branches */
    for(blaux = rblist; blaux; blaux = blaux->Next())
	{
        if(blaux->get_stat() == B_DELETED)
		{
            continue;
		}

        n1 = n2 = n3 = n4 = -1;
        ind1 = ind2 = ind3 = ind4 = -1;

        if(blaux->get_type() == dRES ||
           blaux->get_type() == dCAP ||
           blaux->get_type() == dIND )
		{

			if(blaux->get_node1()->get_real_num())
			{
                n1 = blaux->get_node1()->get_index();
			}

			if(blaux->get_node2()->get_real_num())
			{
				n2 = blaux->get_node2()->get_index();
			}

            if(n1 == -1 && n2 == -1)
			{
                continue;
			}

			/*
            cout<<"Bname:"<<blaux->get_name()
				<<" n1:"<<n1
				<<" n2:"<<n2<<endl;
			*/

            /*          n1      n2 
				  n1    ind1    ind2
				  n2    ind3    ind4
            */
            if(n1 != -1)
			{
                ind1 = matrix->get_label(n1,n1);
                //cout <<"ind1: "<<ind1<<endl;
            }
            if(n1 != -1 && n2 != -1)
			{
                ind2 = matrix->get_label(n1,n2);
                ind3 = matrix->get_label(n2,n1);
                //cout <<"ind2: "<<ind2<<endl;
                //cout <<"ind3: "<<ind3<<endl;
            }
            if(n2 != -1)
			{
                ind4 = matrix->get_label(n2,n2);
                //cout <<"ind4: "<<ind4<<endl;
            } 
            ac_mgr->whole_elem_elim
				(
					blaux->get_name(),
					ind1, ind2, ind3, ind4
				);
        }
        else if( blaux->get_type() == dVCCS ) 
		{
            if(blaux->get_node1()->get_real_num())
			{
                n1 = blaux->get_node1()->get_index();
			}
			if(blaux->get_node2()->get_real_num())
			{
                n2 = blaux->get_node2()->get_index();
			}
			if(blaux->get_node3()->get_real_num())
			{
                n3 = blaux->get_node3()->get_index();
			}
			if(blaux->get_node4()->get_real_num())
			{
                n4 = blaux->get_node4()->get_index();
			}

			/*
            cout<<"Bname:"<<blaux->get_name()
				<<" n1:"<<n1
				<<" n1:"<<n2
				<<" n3:"<<n3
				<<" n4:"<<n4<<endl;
			*/

            /*              n3    n4 
					  n1    ind1    ind2
					  n2    ind3    ind4
            */
            if(n1 != -1 && n3 != -1)
			{
                ind1 = matrix->get_label(n1,n3);
                //cout <<"ind1: "<<ind1<<endl;
            }
            if(n1 != -1 && n4 != -1)
			{
                ind2 = matrix->get_label(n1,n4);
                //cout <<"ind2: "<<ind2<<endl;
            }
            if(n2 != -1 && n3 != -1)
			{
                ind3 = matrix->get_label(n2,n3);
                //cout <<"ind3: "<<ind3<<endl;
            }
            if(n2 != -1 && n4 != -1)
			{
                ind4 = matrix->get_label(n2,n4);
                //cout <<"ind4: "<<ind4<<endl;
            }

            ac_mgr->whole_elem_elim
				(
					blaux->get_name(),
					ind1, ind2, ind3, ind4
				);
            
        }
    }

    print_mesg("End of whole element elimination");
    
	//matrix->print_mna(NUM);
    //matrix->print_mna(DEN);
    
	bool enBranchElim = 1;
    bool useSimpExp = 1;   
	ac_mgr->freq_sweep(useSimpExp, enBranchElim);

}

/*  Perform the eliminations of circuit elements in the complex DDDs
    for both numerator and denominator.  We will remove all the
    appearances of the element admittances with error limits.  We need
    the Branch list of the mian circuit.  we assume the MNA matrix and
    index matrix have been constructed.

*/
void
Circuit::whole_elem_elim_both()
{
    Branch *blaux;
    int  n1,n2,n3,n4;
    int  ind1, ind2, ind3, ind4;

    if(!rblist)
	{
        return;
	}

    print_mesg("Perform whole device elimination for both N and D ... ");

	// first do the freq sweep as we need to do comparison 
	ac_mgr->freq_sweep(FALSE, FALSE);

    /* we go through all the branches */
    for(blaux = rblist; blaux; blaux = blaux->Next())
	{
        if(blaux->get_stat() == B_DELETED)
		{
            continue;
		}

        n1 = n2 = n3 = n4 = -1;
        ind1 = ind2 = ind3 = ind4 = -1;

        if(blaux->get_type() == dRES ||
           blaux->get_type() == dCAP ||
           blaux->get_type() == dIND )
		{

			if(blaux->get_node1()->get_real_num())
			{
                n1 = blaux->get_node1()->get_index();
			}

			if(blaux->get_node2()->get_real_num())
			{
				n2 = blaux->get_node2()->get_index();
			}

            if(n1 == -1 && n2 == -1)
			{
                continue;
			}

#ifdef _DEBUG
            cout<<"Bname:"<<blaux->get_name()
				<<" n1:"<<n1
				<<" n2:"<<n2<<endl;
#endif
            /*       n1      n2 
					 n1    ind1    ind2
					 n2    ind3    ind4
            */
            if(n1 != -1)
			{
                ind1 = matrix->get_label(n1,n1);
                //cout <<"ind1: "<<ind1<<endl;
            }
            if(n1 != -1 && n2 != -1)
			{
                ind2 = matrix->get_label(n1,n2);
                ind3 = matrix->get_label(n2,n1);
                //cout <<"ind2: "<<ind2<<endl;
                //cout <<"ind3: "<<ind3<<endl;
            }
            if(n2 != -1)
			{
                ind4 = matrix->get_label(n2,n2);
                //cout <<"ind4: "<<ind4<<endl;
            } 
            if(ac_mgr->whole_elem_elim_both(blaux->get_name(),
											ind1, ind2, ind3, ind4))
			{
                blaux->get_stat() = B_DELETED;
			}
        }
        else if( blaux->get_type() == dVCCS ) 
		{
            if(blaux->get_node1()->get_real_num())
			{
                n1 = blaux->get_node1()->get_index();
			}
			if(blaux->get_node2()->get_real_num())
			{
                n2 = blaux->get_node2()->get_index();
			}
			if(blaux->get_node3()->get_real_num())
			{
                n3 = blaux->get_node3()->get_index();
			}
			if(blaux->get_node4()->get_real_num())
			{
                n4 = blaux->get_node4()->get_index();
			}

#ifdef _DEBUG
            cout<<"Bname:"<<blaux->get_name()
				<<" n1:"<<n1
				<<" n2:"<<n2
				<<" n3:"<<n3
				<<" n4:"<<n4<<endl;
#endif

            /*        n3      n4 
					  n1    ind1    ind2
					  n2    ind3    ind4
            */
            if(n1 != -1 && n3 != -1)
			{
                ind1 = matrix->get_label(n1,n3);
                //cout <<"ind1: "<<ind1<<endl;
            }
            if(n1 != -1 && n4 != -1)
			{
                ind2 = matrix->get_label(n1,n4);
                //cout <<"ind2: "<<ind2<<endl;
            }
            if(n2 != -1 && n3 != -1)
			{
                ind3 = matrix->get_label(n2,n3);
                //cout <<"ind3: "<<ind3<<endl;
            }
            if(n2 != -1 && n4 != -1)
			{
                ind4 = matrix->get_label(n2,n4);
                //cout <<"ind4: "<<ind4<<endl;
            }

            if(ac_mgr->whole_elem_elim_both(blaux->get_name(),
											ind1, ind2, ind3, ind4))
			{				
                blaux->get_stat() = B_DELETED;
			}
        }
    }

    print_mesg("End of whole device elimination for both");

    //matrix->print_mna(NUM);
    //matrix->print_bool();
    bool enBranchElim = 1;
    bool useSimpExp = 1;
    ac_mgr->freq_sweep(useSimpExp, enBranchElim);
  
}


void
Circuit::statistic()
{
    long node_count = 0;
    long path_count = 0;
    cout <<"----------------------"<<endl;
    cout <<"Ckt: "<<spice_file<<endl;
    cout <<"Title: "<<title<<endl;
    cout <<"#nodes: "<<nsize <<endl;
    //cout <<"#nodes list: "<<endl;
    //nodes->print_nodes(cout);
    cout <<"#branch: "<<bsize <<endl;
    if(ac_mgr->inputs)
		  cout <<"#stimula: "<<ac_mgr->input_num<<endl;
    if(ac_mgr->outputs)
		  cout <<"#output node: "<<ac_mgr->output_num<<endl;

    cout <<"----------------------"<<endl;
    cout <<"magnitude control error(magerr): "<<opMagErr<<endl;
    cout <<"phase control error(phserr): "<<opPhsErr<<endl;
    cout <<"Rel_threshold(relth): "<<opRelThd<<endl;
    cout <<"Rel_threshold(for num coeff)(nrelth): "
		 <<opFCNRelThd<<endl;
    cout <<"Rel_threshold(for den coeff)(drelth): "
		 <<opFCDRelThd<<endl;
    cout <<"coeff suppress th(alpha): "<<opAlpha<<endl;

#ifndef COLLECT_COEFF_DATA
	cout <<"GC for complex DDDs ..." << endl;		
    ac_mgr->cddd_forceGC(); 
#else
	cout <<"GC for fcoeff DDDs ..." << endl;	
    ac_mgr->fcoeff_forceGC();
    //ac_mgr->coeff_forceGC();
#endif
    cout << "---------------------"<<endl;
    cout << "#(top level)total nodes (DDD): " 
		 << ddd->NumNodes() -2 << endl;
    cout << "#(top level)total paths (DDD): " 
		 << ddd->NumCubes() << endl;
    
	node_count += (ddd->NumNodes() - 2);
    path_count += ddd->NumCubes();
    SubCircuit *ckt_aux = sub_ckt;
    
	for(;ckt_aux; ckt_aux = ckt_aux->Next())
	{
        ckt_aux->ddd->GC();
		
		cout << "#total nodes (DDD): " 
			 << ckt_aux->ddd->NumNodes() -2 
			 << endl;
        
		cout << "#total paths (DDD): " 
			 << ckt_aux->ddd->NumCubes() 
			 << endl;
        
		node_count += (ckt_aux->ddd->NumNodes() - 2);
        path_count += ckt_aux->ddd->NumCubes();
    }

    cout << "#total overall nodes (DDD): " << node_count << endl;
    cout << "#total overall pathes (DDD): " << path_count << endl;
    cout << "---------------------"<<endl;
}


/*
 * mark all the branch which are symbolic branch when we read the
 * information from a file.
 */
void
Circuit::mark_symbolic_branchs()
{

	Branch *blaux;	

	if((!rnlist || !rblist) )
	{
        print_error(FAT_ERROR,"No netlist is loaded!");
        return;
    }

	if(theSymbBranchList.size() == 0)
	{
		return;		
	}
	
	for(blaux = rblist; blaux; blaux = blaux->Next())
	{
		// for each branch, we go through the symbolic branch list to
		// see if the branch is is symbolic one.
		for(unsigned int i = 0; i < theSymbBranchList.size(); i++)
		{
			SymbBranch *bch = theSymbBranchList[i];
			
			//cout <<"branch name: " << blaux->get_name() << endl;

			// some branches may be combined, so we need to find the
			// occurance of the given branch name.
			if(strstr(blaux->get_name(), bch->get_name()))
			{
				blaux->set_is_symbolic(true);

				cout <<"branch: " << blaux->get_name() << " is a symbolic branch." << endl;
				break;				
			}			
		}
		
	}

}
