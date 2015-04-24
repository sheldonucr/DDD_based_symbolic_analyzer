
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
 *    $RCSfile: parse_bch.cc,v $
 *    $Revision: 1.4 $
 *    $Date: 2002/09/29 17:04:56 $
 */


/* parse_bch:
 *   Contains parsing functions related to circuit branches,
 *   such as r, l, or c type branches, sources, and active devices.
 *
 */

/* INCLUDES: */
#include "globals.h"
#include "rclcompx.h"

#include "circuits.h"
#include "acan.h"
#include "ckt_call.h"

/* Function definitions: */

/* ------------------------------------------------------------------------
   This procedure parses a resistor, capacitor or inductor 
   Format is XNAME node1 node2 VAR (Symbolic branch name is XNAME)
      where X is r, l, or c
   ------------------------------------------------------------------------ */   
void
Circuit::parse_rlc
(
	char *keyword, 
	char  *n1, 
	char *n2, 
	char *value, 
	char *var_name
)
{
    
    Branch *branch;
    int node1 = atoi(n1);
    int node2 = atoi(n2);

    Node *Nnode1, *Nnode2;
    Nnode1 = query_node(node1);
    if(!Nnode1)
	{
        nsize++;
        Nnode1 = new Node(node1, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode1);
    }

    Nnode2 = query_node(node2);
    if(!Nnode2)
	{
        nsize++;
        Nnode2 = new Node(node2, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode2);
    }

    /* first check lumping condition */
    if((branch = check_lump(Nnode1, Nnode2, WhichType(keyword))))
	{
        branch->lump_branch(keyword, TransValue(value));
        return;
    }

    if(!(branch = query_branch(keyword)))
	{
        if(value)
            branch = new Branch(keyword, 
                WhichType(keyword),
                Nnode1, Nnode2,
                TransValue(value),
                (bsize + 1));
        else
            branch = new Branch(keyword, 
                WhichType(keyword),
                Nnode1, Nnode2,
                var_name,
                (bsize + 1));

        // add into the circuit branch list 
        add_branch(branch);
    }
    else
	{
        sprintf(_buf,"Multiplely defined name: %s,",keyword);
        error_mesg(PARSE_ERROR,_buf);
    }

    // add to each node branch list respectively
    Nnode1->add_branch(branch);
    Nnode2->add_branch(branch);

}

/**********************************************************************
 * parse_vi:
 *
 *   parse an independent voltage or current source...
 *
 *      - parse command line args
 *         - determine source function type and read required parameters
 *           based on the function type
 *         - add branch and nodes to the ckt's nodelist
 *         - add nodes for Iv and E branch equations
 *
 */
 
void 
Circuit::parse_vi
(
	char *keyword, 
	char *node1, 
	char *node2, 
	char *svalue
)
{
  
  double value;
  
  int i_node1 = atoi(node1);
  int i_node2 = atoi(node2);

  if(svalue)
  {	  
	  value = TransValue(svalue);
  }
  else
  {	  
	  value = 1.0;
  } 

  if(i_node1)
  { // add the excitation node
        Node *Nnode1 = query_node(i_node1);
        if(!Nnode1)
		{ // new node
            nsize++;
            Nnode1 = new Node(i_node1, nsize, FALSE, NULL, NULL);
            add_node(Nnode1);
        }
  }
  if(i_node2)
  { // add the excitation node
        Node *Nnode2 = query_node(i_node2);
        if(!Nnode2)
		{
            nsize++;
            Nnode2 = new Node(i_node2, nsize,FALSE,NULL,NULL);
            add_node(Nnode2);
        }
  }
  // create an excitation entry
    IOList *iolist;
    if(tolower(keyword[0]) == 'v')
	{		
        iolist = new IOList(i_node1, i_node2, dVOL, value);
	}	
    else
	{		
        iolist = new IOList(i_node2, i_node1, dCUR, value);
	}	

    ACAnalysisManager::add_input(iolist);
}

/**********************************************************************
 * parse_noise:
 *
 *   parse an independent noise source (voltage or current source...)
 *
 */
 
void 
Circuit::parse_noise
(
	char *keyword, 
	char *node1,
	char *node2,
	char *svalue,
	char *fd
) // fd = 0: independant; 1, 1/f type; 2, special function
{

  double value;

  if(tolower(keyword[0]) != 'v' &&
     tolower(keyword[0]) != 'i' ) 
  {
		sprintf(_buf,"Unknown source : %s, ignored.\n",keyword);
		error_mesg(IO_ERROR,_buf);
		return;
  }

  int  depfreq = atoi(fd);
  int i_node1 = atoi(node1);
  int i_node2 = atoi(node2);

  if(svalue)
     value = TransValue(svalue);
  else
       value = 1.0;

  if(i_node1)
  { // add the excitation node
        Node *Nnode1 = query_node(i_node1);
        if(!Nnode1)
		{ // new node
            nsize++;
            Nnode1 = new Node(i_node1, nsize, FALSE, NULL, NULL);
            add_node(Nnode1);
        }
  }
  if(i_node2)
  { // add the excitation node
        Node *Nnode2 = query_node(i_node2);
        if(!Nnode2)
		{
            nsize++;
            Nnode2 = new Node(i_node2, nsize,FALSE,NULL,NULL);
            add_node(Nnode2);
        }
  }
  // create an excitation entry
    IOList *iolist;
    if(tolower(keyword[0]) == 'v')
	{		
        iolist = new IOList(i_node1, i_node2, dVOL, value, oMEG, depfreq);
	}	
    else if(tolower(keyword[0]) == 'i')
	{	
        iolist = new IOList(i_node2, i_node1, dCUR, value, oMEG, depfreq);
	}	
	else
	{
		sprintf(_buf,"Unknown source : %s, ignored.\n",keyword);
		error_mesg(IO_ERROR,_buf);
		return;
	}

    ACAnalysisManager::add_input(iolist);
}

/* ------------------------------------------------------------------------
   This procedure parses a voltage controlled current or voltage source. 
   Format is XNAME node1 node2 cnode1 cnode2 value (Symbolic  name is XNAME
      where X is g (current) or e (voltage) source.
   ------------------------------------------------------------------------ */
void 
Circuit::parse_vcs
(
	char *keyword, 
	char *n1, 
	char *n2, 
    char *cn1, 
	char *cn2, 
	char *value, 
	char *var_name
)
{
    int node1 , node2 , cnode1 , cnode2 ; 
    Node *Nnode1, *Nnode2,  *Ncnode1, *Ncnode2;

    node1 = atoi(n1);
    node2 = atoi(n2);
    cnode1 = atoi(cn1);
    cnode2 = atoi(cn2);
    
    Nnode1 = query_node(node1);
    if(!Nnode1)
	{
        nsize++;
        Nnode1 = new Node(node1, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode1);
    }

    Nnode2 = query_node(node2);
    if(!Nnode2)
	{
        nsize++;
        Nnode2 = new Node(node2, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode2);
    }

    Ncnode1 = query_node(cnode1);
    if(!Ncnode1)
	{
        nsize++;
        Ncnode1 = new Node(cnode1, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Ncnode1);
    }

    Ncnode2 = query_node(cnode2);
    if(!Ncnode2)
	{
        nsize++;
        Ncnode2 = new Node(cnode2, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Ncnode2);
    }

    /* create the branch here */
    Branch *branch; 
    if(!(branch = query_branch(keyword)))
	{
        if(value)
            branch = new Branch(keyword, 
                    WhichType(keyword),
                    Nnode1, Nnode2,
                    Ncnode1, Ncnode2,
                    TransValue(value),
                    (bsize + 1));
        else
            branch = new Branch(keyword, 
                    WhichType(keyword),
                    Nnode1, Nnode2,
                    Ncnode1, Ncnode2,
                    var_name,
                    (bsize + 1));
    
        /* add the branch into the circuit branch list */
        add_branch(branch);
    }
    else
	{
        sprintf(_buf,"Multiplely defined name: %s,",keyword);
        error_mesg(PARSE_ERROR,_buf);
    }

    /* add branch into circuit controlling node list */
    Nnode1->add_branch(branch);
    Nnode2->add_branch(branch);

    /* note:
    ** do not add branch into the controlling node's branch list 
    */
    /*
    Ncnode1->add_branch(branch);
    Ncnode2->add_branch(branch);
    */

    /* for VCVS, an extra node should be added in MNA formulation*/
    if(WhichType(keyword) == dVCVS)
	{
        nsize++;
        char _name[128];
        sprintf(_name,"I_%s_vcs",ToLower(keyword));
        cout << "extra varible generated: " << _name <<endl;
        Nnode1 = new Node(++extra_var_num, nsize, TRUE, _name, NULL);

        // add the branch into the node incident branch list;
        Nnode1->add_branch(branch);

        // add the node into the branch incident node list
        branch->add_cur_variable(Nnode1);
    }
}
        
/* ------------------------------------------------------------------------
   This procedure parses a current controlled current or voltage source. 
   Format is XNAME node1 node2 cntl_branch value (Symbolic  name is XNAME)
      where X is f (current) or h (voltage) source.
   ------------------------------------------------------------------------ */
void
Circuit::parse_ccs
(
	char *keyword, 
	char *n1,
	char *n2, 
	char *ctrl_branch, 
	char *value, 
	char *var_name
)
{

    int node1 = atoi(n1);
    int node2 = atoi(n2);

    Node *Nnode1, *Nnode2;
    Nnode1 = query_node(node1);
    if(!Nnode1)
	{
        nsize++;
        Nnode1 = new Node(node1, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode1);
    }
    Nnode2 = query_node(node2);
    if(!Nnode2)
	{
        nsize++;
        Nnode2 = new Node(node2, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode2);
    }

    Branch *branch;
    if(!(branch = query_branch(keyword)))
	{
        if(value)
            branch = new Branch(keyword, 
								WhichType(keyword),
								Nnode1, Nnode2,
								TransValue(value),
								(bsize + 1));
        else 
            branch = new Branch(keyword,
								WhichType(keyword),
								Nnode1, Nnode2,
								var_name,
								(bsize + 1));
                        
        
        // add into the circuit branch list 
        add_branch(branch);
    }
    else
	{
        sprintf(_buf,"Multiplely defined name: %s,",keyword);
        error_mesg(PARSE_ERROR,_buf);
    }


    Branch *B_aux = query_branch(ctrl_branch);
    if(!B_aux)
	{
		sprintf(_buf,"Control branch %s does not exist or does not exist before %s in the input desk",
				ctrl_branch, keyword);

        print_error(PARSE_ERROR, _buf);		
    }
    else
	{		
        branch->add_ctrl_branch(B_aux);
	}	

    // add to each node branch list respectively
    Nnode1->add_branch(branch);
    Nnode2->add_branch(branch);

    /* for CCVS, an extra node should be added in MNA formulation*/
    if(WhichType(keyword) == dCCVS)
	{
        nsize++;
        char _name[64];
        sprintf(_name,"I_%s_ccs",ToLower(keyword));
        Nnode1 = new Node(++extra_var_num, nsize, TRUE, _name, NULL);

        // add the branch into the node incident branch list;
        Nnode1->add_branch(branch);

        // add the node into the branch incident node list
        branch->add_cur_variable(Nnode1);
    }
}

/* ------------------------------------------------------------------------
   This procedure parses a mutual indutor statement. 
   Format is KXXXX Lxxx1 Lxxx2 VALUE. Where Lxxxx1 and Lxxxx2 are
   two coupling inductors  source and VALUE is the coupling coefficient.
   ------------------------------------------------------------------------ */
void
Circuit::parse_mutual_inductor
(
	char *keyword, 
	char *l1,
	char *l2, 
	char *value 
)
{

	assert(keyword);
	assert(l1);
	assert(l2);
	assert(value);
	
    Branch *branch;
    if(!(branch = query_branch(keyword)))
	{
        if(value)
		{
			double rvalue = TransValue(value);
			if(rvalue <= 0.0 || rvalue >= 1.0)
			{
				 sprintf(_buf,"%s: invalid mutual coefficient value: %g,",keyword, rvalue);
				 error_mesg(PARSE_ERROR,_buf);
			}			
			
            branch = new Branch(keyword, 
								WhichType(keyword),
								(bsize + 1));			
                                
			// add into the circuit branch list 
			add_branch(branch);
			branch->set_dvalue(rvalue);			
		}		
    }
    else
	{
        sprintf(_buf,"Multiplely defined name: %s,",keyword);
        error_mesg(PARSE_ERROR,_buf);
    }

    Branch *B_aux = query_branch(l1);
    if(!B_aux)
	{ 	
		sprintf(_buf,"inductor %s does not exist or does not exist before %s in the input desk",
				l1, keyword);
        print_error(PARSE_ERROR, _buf);		
    }
    else
	{		
        branch->set_mut_ind1(B_aux);
	}
	
	B_aux = query_branch(l2);
    if(!B_aux)
	{		
		sprintf(_buf,"inductor %s does not exist or does not exist before %s in the input desk",
				l2, keyword);
        print_error(PARSE_ERROR, _buf);		
    }
    else
	{		
        branch->set_mut_ind2(B_aux);
	}

	// add the branch into the branch list of corresponding nodes.
	Node * L1node1 = branch->get_mut_ind1()->get_node1();
	Node * L1node2 = branch->get_mut_ind1()->get_node2();
	Node * L2node1 = branch->get_mut_ind2()->get_node1();
	Node * L2node2 = branch->get_mut_ind2()->get_node2();

	if(L1node1)
	{
		cout <<"L1node1: " << L1node1->get_real_num() << endl;
		
		L1node1->add_branch(branch);		
	}
	if(L1node2)
	{
		cout <<"L1node2: " << L1node2->get_real_num() << endl;

		L1node2->add_branch(branch);		
	}
	if(L2node1)
	{
		cout <<"L2node1: " << L2node1->get_real_num() << endl;

		L2node1->add_branch(branch);		
	}
	if(L2node2)
	{
		cout <<"L2node2: " << L2node2->get_real_num() << endl;

		L2node2->add_branch(branch);		
	}
	
}


/* ------------------------------------------------------------------------
   Following procedures parses active elements and does the following things:
       - checks for errors in the input statement
    - malloc the branch structure and add branch to ckt branch list
    - initialize some branch variables
    - add nodes to the branch structure as appropriate
    - if a user defined subckt
          - add nodes to subckt
    - initialize various and sundry other parameters

   Inputs:  
   Format of line is  (depending on active element type)
    DNAME  node1 node2 modelname  (for DIODE)
    JNAME  node1 node2  node3  modelname (for JFET)
    QNAME  node1 node2  node3  modelname (for BJT)
    MNAME  node1 node2  node3 node4  modelname (for MOSFET)
        
   Outputs: void
   
   Side Effects: 

   ------------------------------------------------------------------------ */ 

/* For DIODE */
void 
Circuit::parse_diode
(
	char *keyword, 
	char *n1, 
	char *n2, 
	char *mname
)
{
    int node1 = atoi(n1);
    int node2 = atoi(n2);

    Node *Nnode1, *Nnode2;
    Nnode1 = query_node(node1);
    if(!Nnode1)
	{
        nsize++;
        Nnode1 = new Node(node1, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode1);
    }
    Nnode2 = query_node(node2);
    if(!Nnode2)
	{
        nsize++;
        Nnode2 = new Node(node2, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode2);
    }
    Branch *branch;
    if(!(branch = query_branch(keyword)))
	{
        branch = new Branch(keyword, 
            WhichType(keyword),
            Nnode1, Nnode2,
            0.0, (bsize + 1));
        
        branch->add_model_name(mname);
    
        // add into the circuit branch list 
        add_branch(branch);
    }
    else
	{
        sprintf(_buf,"Multiplely defined name: %s,",keyword);
        error_mesg(PARSE_ERROR,_buf);
    }

    // add to each node branch list respectively
    Nnode1->add_branch(branch);
    Nnode2->add_branch(branch);

}

/* for BJT */
void 
Circuit::parse_active
(
	char *keyword, 
	char *n1, 
	char *n2, 
	char *n3,
	char *n4, 
	char *mname 
)
{
    int node1, node2, node3, node4;
    node1 = atoi(n1);
    node2 = atoi(n2);
    node3 = atoi(n3);
    if(n4)
	{		
        node4 = atoi(n4);
	}	

    Node *Nnode1, *Nnode2, *Nnode3, *Nnode4 = NULL;

    Nnode1 = query_node(node1);
    if(!Nnode1)
	{
        nsize++;
        Nnode1 = new Node(node1, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode1);
    }

    Nnode2 = query_node(node2);
    if(!Nnode2)
	{
        nsize++;
        Nnode2 = new Node(node2, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode2);
    }

    Nnode3 = query_node(node3);
    if(!Nnode3)
	{
        nsize++;
        Nnode3 = new Node(node3, nsize, FALSE, NULL, NULL);

        // add into the node list of the circuit
        add_node(Nnode3);
    }

    if(n4)
	{
        Nnode4 = query_node(node4);
        if(!Nnode4)
		{
            nsize++;
            Nnode4 = new Node(node4, nsize, FALSE, NULL, NULL);

            // add into the node list of the circuit
            add_node(Nnode4);
        }
    }
    else
        Nnode4 = (Node *)NULL;

    Branch *branch;
    if(!(branch = query_branch(keyword)))
	{
        branch = new Branch(keyword, 
							WhichType(keyword),
							Nnode1, Nnode2,
							Nnode3, Nnode4,
							0.0, (bsize + 1));
        
        branch->add_model_name(mname);
        
        // add into the circuit branch list 
        add_branch(branch);
    }
    else
	{
        sprintf(_buf,"Multiplely defined name: %s,",keyword);
        error_mesg(PARSE_ERROR,_buf);
    }

    // add to each node branch list respectively
    Nnode1->add_branch(branch);
    Nnode2->add_branch(branch);
    Nnode3->add_branch(branch);
    if(n4)
	{		
        Nnode4->add_branch(branch);
	}		
}

/* 
**    function concerning subckt definition 
*/

SubCircuit *
Circuit::parse_subckt
(
	char *ckt_name
)
{
    SubCircuit    *new_ckt = new SubCircuit((SubCircuit *)this, ckt_name);
    theCkt->add_sub_ckt( new_ckt );
    return new_ckt;
}

/*---------------------------------------------
**    parse the sub-circuit interface node list
**---------------------------------------------*/
void
SubCircuit::parse_inter_node
(
	char *_cnode
)
{
    int    node = atoi(_cnode);
    add_inter_node( node );
    cout <<node<<" "<<endl;

    // add into the circuit node list pointed by 'rnlist'
    Node *Nnode;
    Nnode = query_node(node);
    if(!Nnode)
	{
        nsize++;
        Nnode = new Node(node, nsize, FALSE, NULL, NULL);
 
        // add into the node list of the circuit
        add_node(Nnode);
    }
    // the node becoms a boundary node
    Nnode->set_type(nBound);
}

/*---------------------------------------------
**    parse the variable and defualt value pair
**---------------------------------------------*/

void
SubCircuit::parse_deflt_value
( 
	char *_name, 
	char *_cvalue
)
{
    double value = TransValue(_cvalue);
    add_deflt_value(_name,value);
    cout <<"default_var:" <<_name<<"="<<value<<";"<<endl;
}

/* 
**    functions concerning subckt calling 
*/

/*-----------------------------------------------
** initialize the parsing of subckt calling
**----------------------------------------------*/
void 
Circuit::parse_ckt_call
(
	char *keyword
)
{
    CktCall *call_aux = new CktCall(keyword);
    add_ckt_call(call_aux);
}
/*-----------------------------------------------
**  parse the subckt calling interface node list
**  Note: We do not create a branch for subckt 
**     calling state. During MNA setup, we will go
**  through all them statement to generate the
**     corresponding MNA elements.
**----------------------------------------------*/
void
Circuit::parse_ckt_call_node
( 
	char *_cnode
)
{
    if(!cur_call)
	{
        error_mesg(INT_ERROR,"No subckt calling statement.");
        return ;
    }

    int node = atoi(_cnode);

    // add into the interface node list of current subckt call 
    cur_call->add_inter_node( node );

    // add into the circuit node list pointed by 'rnlist'
    Node *Nnode;
    Nnode = query_node(node);
    if(!Nnode)
	{
        nsize++;
        Nnode = new Node(node, nsize, FALSE, NULL, NULL);
 
        // add into the node list of the circuit
        add_node(Nnode);
    }
    // the node becomes a boundary node
    //Nnode->set_type(nBound);

    cout <<node<<" "<<endl;
}
 
/*----------------------------------------
**  parse the parameter and value pair
**---------------------------------------*/
void
Circuit::parse_ckt_call_param( char *_name, char *_cvalue)
{
    if(!cur_call)
	{
        error_mesg(INT_ERROR,"No subckt calling statement.");
        return ;
    }
    double value = TransValue(_cvalue);

    // add the parameter and its value into the subckt corresponding list
    cur_call->add_var_value(_name,value);
    cout <<_name<<"="<<value<<";"<<endl;
}
