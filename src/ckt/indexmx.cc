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
 *    $RCSfile: indexmx.cc,v $
 *    $Revision: 1.5 $
 *    $Date: 2002/09/29 17:04:56 $
 */

 
#define _DEBUG

/* INCLUDES: */
#include "globals.h"
#include <math.h>
#include <limits.h>
 
#include "circuits.h"
#include "mxexp.h"
#include "dtstruct.h"
  
/* this file include some independent routines
   for constructing a IndexMatrix from a file
   and dump matrix into file.
*/

IndexMatrix *theIndMax;
DDDmanager  *theDDDmgr;

/*
    read a matrix from a file.
    Note that the data format for matrix is fixed.
    The first line is the size of the matrix  and each
    following line corresponds a row in the matrix. 
    The first number in each line is the row index
    of that row. Each number following the row index
     denote a element with its row given the row index
    and column given by the number itself.
    Example:
    ---------
    3
    1 1 2
    2 1 2 3
    3 2 3 
    ---------
    This is typical 3x3 band matrix description.
*/

int 
ReadMatrix(char *file)
{
    FILE *fp;
    int  size, row, col;
    char *str, *sbuf, *osbuf;
    DDDnode *theDDD;

    if(!file)
	{
        error_mesg(FAT_ERROR,"Invalide filename.\n");
        return -1;
    }
    fp = fopen(file,"r");
    
	if(!fp)
	{
        sprintf(_buf,"Can't open file: %s\n",file);
        error_mesg(FAT_ERROR,_buf);
        return -1;
    }
    sbuf = osbuf = new char[1024];
    fgets(sbuf,999,fp);
    sscanf(sbuf,"%d",&size);
    cout <<"Size of matrix: "<<size<<endl;
    
    if(!theDDDmgr)
	{
        theDDDmgr = new DDDmanager;
	}
    if(theIndMax)
	{
        delete theIndMax;
	}
    theIndMax = new IndexMatrix(size,theDDDmgr);

    for(int i=0; i<size; i++)
	{
        fgets(sbuf,999,fp);
        str = NextField(&sbuf);
        row = atoi(str);
        while((str = NextField(&sbuf)))
		{
            col = atoi(str);    
            theIndMax->add_element(row, col);
        }
    }
	
    theIndMax->print_bool();
    theIndMax->initial_rowcol();
	IndexMatrix::reset_label();
	theIndMax->min_label_assignR();
	//theIndMax->random_assign();
	theIndMax->force_label_assign();
	theIndMax->print_index();
    theDDD = theIndMax->matrix_recursive_expR(-1,-1);
    theIndMax->ddd_statistic(theDDD);
	//theDDDmgr->PrintDDDTree(theDDD);
    
	delete [] osbuf;

	return 1;
}

/*
    read a matrix from a file.
    Note that the data format for matrix is fixed.
    The first line is the size of matrix  and the
    second line contains the #elements. Following
    lines are the description of the matrix with
    each entry are labeled. The syntax:
    row column label
    ....
    Example:
    ---------
    3
    7
    1 1 1
    1 2 2
    2 1 3
    2 2 4
    2 3 5
    3 2 6
    3 3 7
    ---------
    This is typical 3x3 band matrix description.
*/

int 
ReadMatrixLabel(char *file)
{
    FILE *fp;
    int  size, num_elem, row, col, label;
    char sbuf[1024];	
    DDDnode *theDDD;

    if(!file)
	{
        error_mesg(FAT_ERROR,"Invalide filename.\n");
        return -1;
    }
    fp = fopen(file,"r");
    if(!fp)
	{
        sprintf(_buf,"Can't open file: %s\n",file);
        error_mesg(FAT_ERROR,_buf);
        return -1;
    }
    
    
	fgets(sbuf, 999,fp);
    sscanf(sbuf,"%d",&size);
    cout <<"Size of matrix: "<<size<<endl;
    fgets(sbuf,999,fp);
    sscanf(sbuf,"%d",&num_elem);
    cout <<"#element: "<<num_elem<<endl;
    
    if(!theDDDmgr)
	{
        theDDDmgr = new DDDmanager;
	}
    if(theIndMax)
	{
        delete theIndMax;
	}
    theIndMax = new IndexMatrix(size,theDDDmgr);

    for(int i=0; i<num_elem; i++)
	{
        fgets(sbuf,999,fp);
        sscanf(sbuf,"%d %d %d\n",&row, &col, &label);
        //cout<<"row:"<<row<<"col:"<<col<<"label:"<<label<<endl;
        theIndMax->add_element(row, col);
        theIndMax->add_label(row, col, label);
    }
	
    theIndMax->print_bool();
    theIndMax->initial_rowcol();
	theIndMax->print_index();
    
	double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    double time2;
    double c_time;

	theDDD = theIndMax->matrix_recursive_expR(-1,-1);

	//theDDDMgr->Keep(theDDD);
	//theDDDmgr->GC();
	
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"DDD construction time: " << c_time <<endl;

	theIndMax->ddd_statistic(theDDD);
	
	theDDDmgr->PrintDDDTree(theDDD);    

	return 1;
}
