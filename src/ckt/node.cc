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
 *    $RCSfile: node.cc,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/11/01 20:35:26 $
 */


#include "globals.h"
#include "branch.h"
#include "node.h"

//////////////////////////////////////////////////
// The member function defintion of the Node class
//////////////////////////////////////////////////

Node::Node(int _real_num, int _num, int _isExtraVar, 
            char *_name, Node *_next=NULL )
{
    type    = nNorm;
    real_num = _real_num;
    num = _num;
    isExtraVar = _isExtraVar;
    next = _next;
    lbranch = new BranchList;
    if(isExtraVar)
        name = CopyStr(_name);
    else{
        char buf[64];
        sprintf(buf,"v%d",real_num);
        name = CopyStr(buf);
    }
}

Node::~Node()
{
    delete [] name;
    if(lbranch) delete lbranch;
    if(next) delete next;
}

/*
** add a brnach into the branchlist of the node
*/

void
Node::add_branch(Branch *branch)
{
    lbranch->add_branch(branch);
}

int
Node::check_branches()
{return 1;}

/////////////////////////////////////////////////////////
//    The member function declarations of NodeList class
/////////////////////////////////////////////////////////

/*
**    Search node in current node list
*/

Node *
NodeList::query_node(int node)
{
    DummNode    *dumm_npt = nlist;
    Node         *node_ptr;

    if(nlist)
        return NULL;

    for(;dumm_npt; dumm_npt = dumm_npt->next) {
        node_ptr = dumm_npt->get_node();
        if(node_ptr->get_real_num() == node)
            return node_ptr;
    }
    return (Node *)NULL;
}
    
/* 
**    add a node into created dummy node_list 
**    with the order of adding 
*/

void
NodeList::add_node(Node *_node)
{
    DummNode *dn_aux = nlist;

    if(!_node)
        return;
    
    DummNode *new_nd = new DummNode(_node);
    if(!dn_aux)
        nlist = new_nd;
    else{
        while(dn_aux->next) dn_aux = dn_aux->next;
            dn_aux->next = new_nd;
    }
    num ++;
}

void
NodeList::add_node(Node *_node, double _value)
{
    DummNode *dn_aux = nlist;

    if(!_node)
        return;
    
    DummNode *new_nd = new DummNode(_node, _value);
    if(!dn_aux)
        nlist = new_nd;
    else{
        while(dn_aux->next) dn_aux = dn_aux->next;
            dn_aux->next = new_nd;
    }
    num ++;
}

void
NodeList::print_node_list(ostream & out)
{
    DummNode *dn_aux = nlist;

    out << "Nodes in the list(real_num): ";
    for(; dn_aux; dn_aux = dn_aux->next)
        out <<dn_aux->node->get_real_num() <<" ";
    out <<endl;
}
