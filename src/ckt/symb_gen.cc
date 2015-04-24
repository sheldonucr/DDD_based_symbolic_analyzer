/*
*******************************************************

        Symbolic Circuit Analysis With DDDs
                (*** SCAD3 ***)

   Xiang-Dong (Sheldon) Tan, 2000 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 10/26/1998 -> %G%
*******************************************************
*/

/*
 *    $RCSfile: symb_gen.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/05/20 04:22:01 $
 */

#define _DEBUG

/*
 * sym_gen.c:
 * 
 *  Generate the sequence of symbolic expressions in the sum of
 *  product form.
 *
 * X.Tan Aug. 4, 2000 Monterey Design Systems.
 * 
 */

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
#include "acan.h"
#include "circuits.h"
#include "ckt_call.h"


//static int symb_index = 0;

/* FUNCTIONS */
extern void statistic_info(DDDnode * Z);

//
// Dump the symbolic expression from DDDs.
//
/*
void
ACAnalysisManager::print_expression(ostream & out)
{
  ExpList * poly_list = poly;
  complex    cvalue = 0.0;

  if(!poly_list){
    error_mesg(INT_ERROR,"No symbolic polynomial expressions derived");
    return;
  }
  
  // make sure each cofactor in the subcircuit be visited once.
  theCkt->sub_init_ht();
  
  for(; poly_list ; poly_list = poly_list->next)
    {
      poly_list->print_expression(out);
    }
}


void
ExpList::print_expression(ostream & out)
{
  Cofactor *cofact_aux = num_list;

  if(!num_list || !den_list){
    error_mesg(INT_ERROR,"Invalid num or den cofactor list");
    return;
  }
  
  int i = 1;
  out <<endl;
  out <<"--------- transfer function numerator ------------" << endl;
  for(i = 1; cofact_aux ; i++, cofact_aux = cofact_aux->Next())
    {
      out <<"Cofactor(" << i <<"):"<<endl;
      cofact_aux->print_expression(out);
    }
  
  out <<"--------- transfer function denominator ------------" << endl; 
  cofact_aux = den_list;
  for(i = 1; cofact_aux ; i++, cofact_aux = cofact_aux->Next())
    {
       out <<"Cofactor(" << i <<"):"<<endl;
      cofact_aux->print_expression(out);
    }
}

void
Cofactor::print_expression(ostream & out)
{
  if(!cddd)
    return;

  if(osign < 0)
    out <<" - " << endl;
  else
    out << " + " << endl;

 
  myStack<CompElemList> compElemListList;

  out <<" { " << endl;
  ddd_mgr->print_paths(cddd, out, compElemListList);
  out <<" }; " << endl;

  if(!compElemListList . isEmpty())
    out << " where: " << endl;
  while(!compElemListList . isEmpty())
    {
      CompElemList *compElemList = compElemListList . pop();
      compElemList -> print_expression(out);
    }
  
}
*/

void
Cofactor::addSymbName(char *name)
{
  if(symb_name)
    return;
  symb_name =  new char[strlen(name) + 1];
  strcpy(symb_name, name);
}

/*
void
MNAElement::print_elem(ostream & out,
		       myStack<CompElemList> & compList)
{
 
  char buffer[128];

  if(clist && !clist_sname){
    sprintf(buffer,"E%d%d_%d\0", get_row(), get_col(), symb_index++);
    clist_sname = new char[strlen(buffer) + 1];
    strcpy(clist_sname,buffer);
    clist -> addSymbName(buffer);
    compList . push (clist);
  }

  if(clist)
    out << "(";
  blist->print_branch(out);

  if(clist){
    out <<" - " << clist_sname <<") ";
  }
}
*/


/*
** Go through all the 1-paths in the given
*/

/*
void
DDDmanager::print_paths(DDDnode *s,
			ostream &out,
			myStack<CompElemList> & compList)
{

  DDDnode *path = Base();

  if(!s)
    return;

 if(s == Base())
    {   
      out << "  1 "<< endl;
      return;
    }
  
 if(s == Empty())
   {
     out << "  0 " << endl;
     return;
   }
  print_pathsR(s, out, path, compList);
}

void
DDDmanager::print_pathsR(DDDnode *s,
			 ostream &out,
			 DDDnode *path,
			 myStack<CompElemList> & compList)
{
    if(!s)
        return;

    if(s == zero)
      return;
    else if( s == one ) 
      {
	print_path(out, path, compList);
	return;
      }

    if(s->Then())
      {
	path = Change(path, s->index);
	print_pathsR(s->Then(), out, path, compList);
	path = Subset1(path, s->index);
      }
    
    if(s->Else())
      print_pathsR(s->Else(), out, path, compList);
}

void
DDDmanager::print_path(ostream & out,
		       DDDnode *path,
		       myStack<CompElemList> & compList)
{
 
  assert(path);

  DDDnode *node = path;
  int sign = 1;

  // statistic(path, out);

  // first the sign of the path
  for(;node->Then() != one; node = node->Then())
    sign *= CalcSign(node, node->Then());

  if(sign > 0)
    out << "\t + ";
  else
    out << "\t - ";
 
  node = path;
  for(;node != one; node = node->Then())
    {
      label2elem->get_elem(node->index)->print_elem(out, compList);
    }
   out << endl;
   
}
*/


