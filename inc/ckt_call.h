/*
 *    ckt_call.h
 *    class def related to sub-circuit calling
*/

/*
 *    $RCSfile: ckt_call.h,v $
 *    $Revision: 1.5 $
 *    $Date: 2002/09/30 20:55:21 $
 */


#ifndef _CKT_CALL_H
#define _CKT_CALL_H

#include "globals.h"
#include "branch.h"
#include "node.h"

class NList;
class DVarList;
class SubCircuit;
class Cofactor;
class HasherTree;

class CktCall {

  char        *call_name; // the name of the instance  
  char        *ckt_name; // the name of subckt called

protected:
  NList        *interface; // the calling node interface
  DVarList     *var_list;  // the parameter list
  SubCircuit   *sub_ckt;    // the corresponding sub-circuit
  CktCall      *next;        

  friend class SubCircuit;
  friend class Circuit;
  friend class MNAMatrix;

public:
  CktCall( char *_call_name);
  ~CktCall();
  void add_inter_node( int node );
  void add_var_value(char *_name, double _value );
  void match_sub_ckt(char *_ckt_name, SubCircuit *_sub_ckt_list);

  CompElemList *build_expression( int _row, int _col);
  MNAElement *find_element( int _row, int _col);

  int p2c(int _node);// node real number mapping from parent ckt to child ckt
  int c2p(int _node);// node real number mapping from child ckt to parent ckt
  void expand_subckt();
  CktCall    *Next() { return next;}
};

class CompElem {

  MNAElement    *A_left;
  MNAElement    *A_right;
  Cofactor      *delta;
  CompElem      *next;
  char *         delta_symb;

  friend class CompElemList;
public:
  CompElem( MNAElement *_left, MNAElement *_right, Cofactor *_cof );
  CompElem( CompElem *);
  ~CompElem();
  CompElem  *Next() { return next;}
  complex   evaluate(double, HasherTree *, HasherTree *, bool); 
  void addSymbName(char *, int);
  void print_elem(ostream & out, HasherTree *ht);
};

class CompElemList {

  CompElem      *list;
  int           num_elem;
  SubCircuit    *subckt;
  CompElemList  *next;
  char          *listSymbName;

public:
  CompElemList( SubCircuit *_ckt );
  ~CompElemList();

  void             add_elem( CompElem *_elem );
  CompElemList*    copy_list();
  CompElem *       get_list() { return list;}
  int              get_num() { return num_elem;}
  complex          evaluate( double freq); 

  CompElemList *   Next() { return next;}
  void             Next(CompElemList *_next) { next = _next;}

  void             addSymbName(char *name);
  void             print_expression(ostream & out);

};

#endif //_CKT_CALL_H
