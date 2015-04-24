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
 *    $RCSfile: subckt.cc,v $
 *    $Revision: 1.8 $
 *    $Date: 2002/11/21 02:40:15 $
 */


/*
**    function related to sub-circuit calling
*/

#include "circuits.h"
#include "ckt_call.h"
#include "acan.h"

///////////////////////////////////////////////
// member function definition of SubCircuit
///////////////////////////////////////////////

SubCircuit::
SubCircuit(SubCircuit *_parent, char *_name):Circuit()
{
    parent      = _parent;
    name    	= CopyStr(ToLower(_name));
    ddd         = ddd = new DDDmanager;
    matrix      = new MNAMatrix(this, ddd);
    ac_mgr      = new ACAnalysisManager(matrix, ddd);
    interface   = NULL;
    var_list    = NULL;
    num_infs_node     = 0;
    num_param    = 0;
    visit_flag	= 0;
    cof_list    = NULL;
    ht 		= NULL;
    next        = NULL;
}

SubCircuit::~SubCircuit()
{
    delete [] name; name = NULL;
    delete ddd; ddd = NULL;
    delete matrix; matrix = NULL;
    delete ac_mgr; ac_mgr = NULL;
    if(interface) delete interface;
    if(var_list) delete var_list;
    
	if(ht) delete ht;
	if(prec_ht) delete prec_ht;

	if(cof_list)
	{
        Cofactor *caux;
        Cofactor *clist = cof_list;
        while(clist)
		{
            caux = clist->Next();
            delete clist;
            clist = caux;
        }
    }
}

/*
** add a node into interface list of the sub circuit
** the resulting order must be the order of adding 
*/

void
SubCircuit::add_inter_node( int _node )
{
    NList    *laux = interface;
    NList    *lnew = new NList(_node);
    if(laux){
        while(laux->next) laux = laux->next;
        laux->next = lnew;
    }
    else
        interface = lnew;
    num_infs_node++;
}

/*
**    add an variable and its default value into the variable list.
*/

void
SubCircuit::add_deflt_value(char *_name, double _value )
{
    DVarList    *vaux = var_list;
    DVarList    *vnew = new DVarList(_name, _value);
    if(vaux){
        while(vaux->next) vaux = vaux->next;
        vaux->next = vnew;
    }
    else
        var_list = vnew;
    num_param++;
}

/* add a cofactor into the cofactor list */
void 
SubCircuit::add_cofactor( Cofactor *_cof)
{
    if(!cof_list)
        cof_list = _cof;
    else{
        _cof->Next(cof_list);
        cof_list = _cof;
    }
}

/* 
    find a cofactor, given the row and col 
    which are the index number of nodes.
*/

Cofactor *
SubCircuit::query_cofactor(int _row, int _col)
{
    Cofactor *cof_aux = cof_list;
    if(!cof_list)
        return NULL;
    else{
        for(;cof_aux;cof_aux = cof_aux->Next()){
            if((cof_aux->get_row() == _row )
            && (cof_aux->get_col() == _col))
                return cof_aux;
        }
    }
    return NULL;
}

/*
**	initialize the hashtree
*/
void
SubCircuit::init_ht()
{
    if(ht)
	{
        delete ht;
	}
	if(prec_ht)
	{
		delete prec_ht;
	}

    ht = new HasherTree(FALSE);
    assert(ht);

	prec_ht = new HasherTree(FALSE);
	assert(prec_ht);

    // initialize the visit_flag for the subcircuit 
    visit_flag = !cof_list->get_cddd()->GetVisitFlag();
}

/*
    Perform the elimination of cancellation terms
    in fcoefficient.
    We need the Branch list of the mian circuit.
    we assume the MNA matrix and index matrix have
    been constructed.
*/

///////////////////////////////////////////////
// member function definition of class CompElemList
///////////////////////////////////////////////

CktCall::CktCall( char *_call_name)
{
    call_name    = CopyStr(ToLower(_call_name));
    ckt_name    = NULL;
    interface    = NULL;
    var_list    = NULL;
    sub_ckt        = NULL;
    next        = NULL;
}

CktCall::~CktCall()
{
    if(call_name)
        delete [] call_name;
    if(ckt_name)
        delete [] ckt_name;
    if(interface)
        delete interface;
    if(var_list)
        delete var_list;
}

/*
** add a node into interface list of the sub circuit
** the resulting order must be the order of adding 
*/

void
CktCall::add_inter_node( int _node )
{
    NList    *laux = interface;
    NList    *lnew = new NList(_node);
    if(laux){
        while(laux->next) laux = laux->next;
        laux->next = lnew;
    }
    else
        interface = lnew;
}

/*
**    add a variable and its default value into the variable list.
*/

void
CktCall::add_var_value(char *_name, double _value )
{
    DVarList    *vaux = var_list;
    DVarList    *vnew = new DVarList(_name, _value);
    if(vaux){
        while(vaux->next) vaux = vaux->next;
        vaux->next = vnew;
    }
    else
        var_list = vnew;
}

/* 
** find the sub circuit with the same name and 
** add it into the subckt calling.
*/
void
CktCall::match_sub_ckt(char *_ckt_name, SubCircuit *_sub_ckt_list)
{
    if(!_sub_ckt_list){
        error_mesg(INT_ERROR,"No preceeding subckt definition");
        return;
    }
    ckt_name    = CopyStr(ToLower(_ckt_name));
    SubCircuit    *laux = _sub_ckt_list;
    for(;laux; laux = laux->next){
        if(!strcmp(_ckt_name,laux->name)){
            sub_ckt    = laux;
            return;
        }
    }
    sprintf(_buf,"No matched subckt for calling:%s",_ckt_name);
    error_mesg(FAT_ERROR,_buf);
}


/*----------------------------------------------------
    FuncName:
    Spec: build composite expression for element (row,col)
    in parent's circuit.
 -----------------------------------------------------
    Inputs: Row anc Column in real number of parent's
            circuit. So, we need transfer it first
            into the real number in children ckt. 
    Outputs:

    Global Variblas used:
----------------------------------------------------*/
CompElemList *
CktCall::build_expression(int _row, int _col)
{
    int sub_row = p2c(_row);
    int sub_col = p2c(_col);
    if(_row == -1 || _col == -1){
        sprintf(_buf,"Unmatched boundary nodes: %d, %d.",_row, _col);
        error_mesg(INT_ERROR,_buf);
        return NULL;
    }
    return sub_ckt->matrix->build_comp_expr( sub_row, sub_col);
}
    
/*----------------------------------------------------
Spec: fine the mna specified element in the subckt 
-----------------------------------------------------*/
MNAElement *
CktCall::find_element( int _row, int _col)
{
    int  sub_row = p2c(_row);
    int  sub_col = p2c(_col);
    if(_row == -1 || _col == -1){
        sprintf(_buf,"Unmatched boundary nodes: %d, %d.",_row, _col);
        error_mesg(INT_ERROR,_buf);
        return NULL;
    }
    return sub_ckt->matrix->get_mna_element_real( sub_row, sub_col);
}

/*----------------------------------------------------
Spec: node's coordinate transformation(from parent to child)
      both node in parent and children are in real_num
      Upon failure, -1 will return.
-----------------------------------------------------*/
int 
CktCall::p2c(int _node)
{ 
    NList    *plist = interface;
    NList    *clist = sub_ckt->interface;

    for(; clist && plist; clist = clist->Next(),plist = plist->Next()){
        if(plist->get_node() == _node)
            return clist->get_node();
    }
    if( _node == 0)
	return 0;
    return -1;
}

int 
CktCall::c2p(int _node)
{ 
    NList    *plist = interface;
    NList    *clist = sub_ckt->interface;

    for(; clist && plist; clist = clist->Next(),plist = plist->Next()){
        if(clist->get_node() == _node)
            return plist->get_node();
    }
    if( _node == 0)
	return 0;
    return -1;
}

/*----------------------------------------------------
Spec: expand the subckt in its upper level circuit
-----------------------------------------------------*/
static int node_count = 0;
void 
CktCall::expand_subckt()
{
    Branch  *baux = sub_ckt->rblist;
    printf("Expand %s(%s) ...\n",call_name,ckt_name);
    node_count += 100;
    int n1, n2, n3, n4;
    for(;baux;baux = baux->Next()){
        switch(baux->get_type()){
            case dVCVS:
            case dVCCS:
                n1 = baux->get_node1()->get_real_num();
                n2 = baux->get_node2()->get_real_num();
                n3 = baux->get_node3()->get_real_num();
                n4 = baux->get_node4()->get_real_num();
                if(c2p(n1) != -1)
                    n1 = c2p(n1);
                else 
                    n1 += node_count;
                if(c2p(n2) != -1)
                    n2 = c2p(n2);
                else
                    n2 += node_count;
                if(c2p(n3) != -1)
                    n3 = c2p(n3);
                else
                    n3 += node_count;
                if(c2p(n4) != -1)
                    n4 = c2p(n4);
                else
                    n4 += node_count;
                printf("%s_%s %d %d %d %d %g\n",baux->get_name(),
                            call_name,n1,n2,n3,n4,baux->get_dvalue());
                break;
            default:
                n1 = baux->get_node1()->get_real_num();
                n2 = baux->get_node2()->get_real_num();
                if(c2p(n1) != -1)
                    n1 = c2p(n1);
                else
                    n1 += node_count;
                if(c2p(n2) != -1)
                    n2 = c2p(n2);
                else
                    n2 += node_count;
                printf("%s_%s %d %d %g\n", 
		    baux->get_name(),call_name,n1,n2,baux->get_dvalue());
        }
    }
}

///////////////////////////////////////////////////////
//    member function definition of class CompElem
//////////////////////////////////////////////////////
CompElem::CompElem(MNAElement *_left, MNAElement *_right, Cofactor *_cof)
{
    A_left    = _left;
    A_right    = _right;
    delta    = _cof;
    next = NULL;
    delta_symb = NULL;
}

CompElem::~CompElem()
{
    //delete A_left;
    //delete A_right;
    //delete delta; //cofactor will be deleted through cofactor list.
  if(delta_symb)
    delete delta_symb;
}

CompElem::CompElem(CompElem *elem)
{
    A_left    = elem->A_left;
    A_right    = elem->A_right;
    delta    = elem->delta;
    next = NULL;
}

complex
CompElem::evaluate
(
	double freq, 
	HasherTree *prec_ht, 
	HasherTree *ht, 
	bool visit_flag
)
{
    complex value;    

    value = A_left->elem_value(freq) * 
            A_right->elem_value(freq) *
            delta->evaluate(freq, prec_ht, ht, visit_flag);
	
    return value;
}

void
CompElem::addSymbName(char *listSymbName, int index)
{
  char buffer[128];

  if(strlen(listSymbName) > 120)
    {
      cout << listSymbName << endl;
      assert(strlen(listSymbName) <= 120);
    }

  sprintf(buffer, "cof_%s_%d\0", listSymbName, index);
  delta_symb =  new char[strlen(buffer) + 1];
  strcpy(delta_symb,buffer);
  // delta->addSymbName(delta_symb);
}

/*
void
CompElem::print_elem(ostream & out, HasherTree *ht)
{
  myStack<CompElemList> compElemListList;

  out << " [";
  A_left->print_elem(out, compElemListList);
  out << "] * [";
  A_right->print_elem(out, compElemListList);
  out << "] * "  << delta_symb
      << ";" << endl;

  
  if(ht->Put(*(delta->get_cddd()), delta->get_cddd())) 
    {
      // out << "row: " << delta->get_row() << " col: " << delta->get_col() << endl;
      out << " where:" << endl;
      out << " " << delta_symb << " = " << endl;
      out << "{" << endl;
      delta->print_expression(out);
      delta->addSymbName(delta_symb);
      out << "};" << endl;
    }
  else
    {
      // out << "row: " << delta->get_row() << " col: " << delta->get_col() << endl;
      out << " where:" << endl;
      out << "{" << endl;
      assert(delta->get_symb_name());
      out << delta_symb << " = " << delta->get_symb_name();
      out << endl << "};" << endl;
    }
  
  if(!compElemListList . isEmpty())
     out << " where: " << endl;
  while(!compElemListList . isEmpty())
    {
      CompElemList *compElemList = compElemListList . pop();
      compElemList -> print_expression(out);
    }
}
*/

///////////////////////////////////////////////////////
//    member function definition of class CompElemList
//////////////////////////////////////////////////////

CompElemList::CompElemList( SubCircuit *_ckt)
{
    subckt = _ckt;
    list = NULL;
    num_elem = 0;
    next = NULL;
    listSymbName = NULL;
}

CompElemList::~CompElemList()
{
    CompElem *aux, *paux;
    if(list){
	aux = paux = list;
	while(aux){
	    paux = aux->next;
	    delete aux,
	    aux = paux;
	}
    }
}

/*
  Make a copy of itself
*/

CompElemList *
CompElemList::copy_list()
{
    if(!list)
	return NULL;

    CompElemList *newlist = new CompElemList(subckt);
    CompElem *aux, *naux;
    for(aux = list; aux; aux = aux->next){
	naux = new CompElem(aux);
	newlist->add_elem(naux); 
    }
    return newlist;
}

/*----------------------------------------------------
Spec: add a CompElem into the list with order of adding
-----------------------------------------------------*/
void
CompElemList::add_elem( CompElem *_elem )
{
    //CompElem *laux = list;
    if(!list){
        list = _elem;
        num_elem++;
        return;
    }
    _elem->next = list;
    list = _elem;
    /*
    while(laux->Next()) laux = laux->Next();
    laux->next = _elem;
    */
    num_elem++;
}    

/*----------------------------------------------------
Spec: evaluate the CompElemList given a freq.
-----------------------------------------------------*/
complex
CompElemList::evaluate(double freq)
{
    CompElem    *laux = list;
    complex     value = 0.0, sys_ddd_value;
    //HasherTree  *ht    = new HasherTree(FALSE);

    while(laux)
	{
        value += laux->evaluate(freq, 
								subckt->get_prec_ht(),
								subckt->get_ht(), 
								subckt->get_visit_flag());
        laux = laux->next;
    }

    sys_ddd_value = subckt->matrix->evaluate_sys_det
		(
			freq, 
			subckt->get_ht()
		);

    //delete ht;
    return value/sys_ddd_value;
}


void
CompElemList::addSymbName(char *name)
{
  assert(name);
  assert(!listSymbName);
  listSymbName =  new char[strlen(name) + 1];
  strcpy(listSymbName, name);

  // give each complex elemenet an unique name 
  CompElem    *laux = list;
  int index = 1;
 
  while(laux){
    laux->addSymbName(listSymbName, index);
    laux = laux->next;
    index ++;
  }
}


/*
void
CompElemList::print_expression(ostream & out)
{
  CompElem    *laux = list;

  out << " -------------- begin of " << listSymbName << " --------------" << endl;
  out << " " << listSymbName << " = " << endl;
  out << " numerator("<<listSymbName << ") = " << endl;
  while(laux){
    laux->print_elem(out,subckt->get_ht());
    if(laux->next)
      out << endl << " + " << endl;
    laux = laux->next;
  }
  out << " ---------------------------" << endl;

  out << " denominator("<<listSymbName << ") = " << endl;
  DDDnode *sys_det = subckt->matrix->get_sys_det();
  assert(sys_det);
  
  if(!subckt->get_ht()->Put(*sys_det, sys_det)) // Already visited
    {
      out <<" system_det_of_" << subckt->get_name() << endl;
      out << " -------------- end of " << listSymbName << " ----------------" << endl;
      return;
    }
  
  out <<" system_det_of_" << subckt->get_name() << "=" << endl;

  myStack<CompElemList> compElemListList;
  out <<" { " << endl;
  subckt->get_ddd()->print_paths(sys_det, out, compElemListList);
  out <<" }; " << endl;

  if(!compElemListList . isEmpty())
    out << " where: " << endl;
  while(!compElemListList . isEmpty())
    {
      CompElemList *compElemList = compElemListList . pop();
      compElemList -> print_expression(out);
    }
  out << " -------------- end of " << listSymbName << " ----------------" << endl;
}
*/
