#ifndef NODE_H
#define NODE_H

/*
 *    $RCSfile: node.h,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/09/30 20:55:21 $
 */

#include <stdio.h>
#include <iostream.h>



/*
 * node.h:
 *
 *    This file contains the node list data structures and the 
 *    routines for manipulating nodes and node lists.
 */

typedef enum 
{
    nNorm,    // no boundary node
    nBound    // boundary node(net) which connects the ckt and its children
} NodeType;

class BranchList;
class Branch;
class Circuit;

class Node {

	int index;   /* node's index in MNA matrix, it must be consecutive */
	
	NodeType type;/* nNorm or nBound */
	
	int num;   /* unique internal node number.
				  num = 1 for ground node.  */

	char *name; /* the variable name for this node */

	int real_num; /* real node number:
					 for circuit nodes, this is the node number 
					 read during parsing.  For extra variable 
					 nodes, this is assigned in the order 
					 created starting with largest num.  */

	int isExtraVar; /* flag to indicate extra variable node.
					   Extra variable nodes are used for 
					   branch current variables and other
					   non-voltage nodes.  */

protected:

	BranchList    *lbranch; /* the incident branch of the node */
	Node *next ;

    friend class NodeList;
    friend class DummNode;
    friend class Circuit;

public:

    Node(int real_num, int num, int isExtraVar, char *name, Node *next );
    ~Node();

    void set_type( NodeType _type) { type = _type;}
    NodeType get_type() { return type; } 

    BranchList *get_lbranch() { return lbranch;}
    int    get_real_num() {return real_num;}
    int get_num() {return num;}
    int & get_index() {return index;}

    void add_branch(Branch *branch); 
    int check_branches();

    Node *Next() {return next;}
    void Next(Node *_next) {next = _next;}

};

class DummNode {
    
protected:
	Node         *node;
	DummNode    *next;
	double        value;    // the excitation value for input node
    
    friend class NodeList;
public:
	DummNode(Node *_node) { node = _node; next = NULL;}
	DummNode(Node *_node, double _value) 
		{ node = _node; value = _value; next = NULL;}
	~DummNode(){ if(next) delete next;}

	Node *   get_node()    { return node;}
	DummNode *Next()    { return next;}
	void     Next(DummNode *_next) { next = _next;}
};

class NodeList 
{
    DummNode     *nlist;
    int            num;

public:

    NodeList() { nlist = NULL; num = 0; }
    ~NodeList(){ delete nlist;}

    int    get_num() { return num; }
    DummNode *get_nlist() const { return nlist;}
    Node *query_node(int node);    // check the existance of a node in 

	// the node list
    void add_node(Node *_node); // sorted by real_num
    void add_node(Node *_node, double _value); // sorted by real_num

    // to see if there are dupicated node(in num or real_num)
    int chk_node_list(){ return 1;} 

    void print_node_list( ostream & out);
};

#endif //NODE_H
