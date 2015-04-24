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
 *    $RCSfile: buildexp1.cc,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/11/01 20:35:26 $
 */


#include <math.h>
#include "circuits.h"
#include "control.h"
#include "acan.h"
#include "mxexp.h"
#include "mna_form.h"
#include "graph_out.h"
#include "approx.h"
#include "control.h"

/*
**	runtines for deriving the symbolic expresions
**	according to the inputs and outputs in the i
**	nput circuits.
*/


/*----------------------------------------------------
    FuncName: build_expresion
    Spec: generate the polynormial expressions of 
          cofacotors represented by DDD
 -----------------------------------------------------
    Inputs: command
    Outputs: the output expression.

    The transfer functions are limited to
    voltage gain -- Vo/Vi or transresistance -- Vo/Ii
	in the current implementation.

    (1) Voltage gain.
    Suppose we have N independent voltage source, 
    for each input source Vsi,ti and
    output voltage Vo1,o2, the voltage gain will be.

    G = Sigma(vi*(Vo1,i/(Vsi - Vti))) - Signm(vi*(Vo2,i/(Vsi-Vti))), 
    i = 1...N,

    Sigma stands for the summation.

    (2) Transimpedence gain
    R = sigma(Vo1,i) - sigma(Vo2,i);

    For input current source Ii with current direction 
	from node1 = si and node2 = ti and
    output node k. According to the cramer's rule:

    Vk = (Ii(-1)^(si+k)det(Tsi,k) - Ii(-1)^(ti+k)det(Tti,k)) / det(T);

    Global Variblas used:
----------------------------------------------------*/
void
ACAnalysisManager::build_expressions()
{
    IOList *in_aux = inputs;
    IOList *out_aux = outputs;    
	int	sign;
	int	ecount = 1; // polynomial expression counter;

    if(!in_aux || !out_aux){
        error_mesg(INT_ERROR,"No excitation node or output node !");
        return;
    }

    if(!command) {
        error_mesg(INT_ERROR,"No AC analysis command given");
        return;
    }

    if(!matrix->index_mat){
        error_mesg(INT_ERROR,"Build MNA matrix first");
        return;
    }

    if(!matrix->sys_det)
        build_sys_ddd();

    print_mesg("BUILDING SYMBOLIC EXPRESSIONS ...");
        
    // now we only process one pair of output terminals

    int out_node1 = matrix->real2label(out_aux->node1);
    int out_node2 = matrix->real2label(out_aux->node2);

    // first, we process output node1
    int in_node1, in_node2;
    if(out_node1 != -1){ // not a ground node

		// build expression for each independent source 
        for(;in_aux; in_aux = in_aux->next){

            in_node1 = matrix->real2label(in_aux->node1);
            in_node2 = matrix->real2label(in_aux->node2);

            // create a new expression and add it into the poly list
            ExpList *expr_aux = 
                new ExpList( in_aux, num_pts, ecount++, in_aux->value); 

            add_exprs(expr_aux);

            Cofactor *cof_aux;

            // the numerator (for Vo1,1)

            if(in_node1 != -1){ // not a ground node
	        DDDnode *ddd = build_cofactor_ddd(in_node1, out_node1);
		if(ddd){
                cof_aux = new Cofactor(ddd_mgr,ddd,
                    PLUS, in_node1, out_node1 );
                expr_aux->add_num_cofactor(cof_aux);
		}
            }
            if(in_node2 != -1){ // not a ground node
	        DDDnode *ddd = build_cofactor_ddd(in_node2, out_node1);
		if(ddd){
                cof_aux = new Cofactor(ddd_mgr,ddd,
                    MINUS, in_node2, out_node1 );
                expr_aux->add_num_cofactor(cof_aux);
		}
            }

            // the denominator

            if(in_aux->type == dVOL){ // voltage type excitation

                // calculate Vsi - Vti

                // first Vsi

                if(in_node1 != -1){ // not a ground node
		    DDDnode *ddd = build_cofactor_ddd(in_node2, in_node1);
		    if(ddd){
                    cof_aux = new Cofactor(ddd_mgr,ddd,
                        PLUS, in_node1, in_node1);
                    expr_aux->add_den_cofactor(cof_aux);
		    }
                }
                if((in_node1 != -1) && (in_node2 != -1)){ // not a ground node
		    DDDnode *ddd = build_cofactor_ddd(in_node2, in_node1);
		    if(ddd){
                    cof_aux = new Cofactor(ddd_mgr,ddd,
                        MINUS, in_node2, in_node1);
                    expr_aux->add_den_cofactor(cof_aux);
		    }
                }

                // then Vti

                if((in_node1 != -1) && (in_node2 != -1)){ 
				    // not a ground node
		    DDDnode *ddd = build_cofactor_ddd(in_node1, in_node2);
		    if(ddd){
                    cof_aux = new Cofactor(ddd_mgr,ddd,
                        MINUS, in_node1, in_node2);
                    expr_aux->add_den_cofactor(cof_aux);
		    }
                }
                if(in_node2 != -1){ // not a ground node
		    DDDnode *ddd = build_cofactor_ddd(in_node2, in_node2);
		    if(ddd){
                    cof_aux = new Cofactor(ddd_mgr,ddd,
                        PLUS, in_node2, in_node2);
                    expr_aux->add_den_cofactor(cof_aux);
		    }
                }
            }
            else if(in_aux->type == dCUR){
		cof_aux = new Cofactor(ddd_mgr,
		matrix->sys_det, PLUS, 1, 1);
		expr_aux->add_den_cofactor(cof_aux);
	    }
        }
    }

    /* 
    ** We process the second output node (out_node2)
    ** The only difference is the sign of each expression
    ** (i.e. They are opposite to the sign of the pervious 
    ** expressions.
    */
    if(out_node2 != -1){ // not a ground node
        for(;in_aux; in_aux = in_aux->next){

            in_node1 = matrix->real2label(in_aux->node1);
            in_node2 = matrix->real2label(in_aux->node2);

            // create a new expression and add it into the poly list
            ExpList *expr_aux = new 
		ExpList( in_aux, num_pts, ecount++, (-1)*in_aux->value);

            add_exprs(expr_aux);

            Cofactor *cof_aux;
            // first calculate Vo1,i
            if(in_node1 != -1){ // not a ground node
	        DDDnode *ddd = build_cofactor_ddd(in_node1, out_node2);
		if(ddd){
                cof_aux = new Cofactor(ddd_mgr,ddd,
                    PLUS, in_node1, out_node2 );
                expr_aux->add_num_cofactor(cof_aux);
		}
            }
            if(in_node2 != -1){ // not a ground node
	        DDDnode *ddd = build_cofactor_ddd(in_node2, out_node2);
		if(ddd){
                cof_aux = new Cofactor(ddd_mgr,ddd,
                    MINUS, in_node2, out_node1 );
                expr_aux->add_num_cofactor(cof_aux);
		}
            }

            if(in_aux->type == dVOL){ // voltage type excitation
                // calculate Vsi - Vti
                // first Vsi
                if(in_node1 != -1){ // not a ground node
		    DDDnode *ddd = build_cofactor_ddd(in_node1, in_node1);
		    if(ddd){
                    cof_aux = new Cofactor(ddd_mgr, ddd,
                        PLUS, in_node1, in_node1 );
                    expr_aux->add_den_cofactor(cof_aux);
		    }
                }
                if((in_node2 != -1) && (in_node1 != -1)){ // not a ground node
		    DDDnode *ddd = build_cofactor_ddd(in_node2, in_node1);
		    if(ddd){
                    cof_aux = new Cofactor(ddd_mgr,ddd,
                        MINUS, in_node2, in_node1 );
                    expr_aux->add_den_cofactor(cof_aux);
		    }
                }

                // then Vti
                if((in_node1 != -1) && (in_node2 != -1)){ // not a ground node
		    DDDnode *ddd = build_cofactor_ddd(in_node1, in_node2);
		    if(ddd){
                    cof_aux = new Cofactor(ddd_mgr, ddd,
                        MINUS, in_node1, in_node2 );
                    expr_aux->add_den_cofactor(cof_aux);
		    }
                }
                if(in_node2 != -1){ // not a ground node
		    DDDnode *ddd = build_cofactor_ddd(in_node2, in_node2);
		    if(ddd){
                    cof_aux = new Cofactor(ddd_mgr,ddd,
                        PLUS, in_node2, in_node2 );
                    expr_aux->add_den_cofactor(cof_aux);
		    }
                }
            }
            else if(in_aux->type == dCUR){
		printf("current source\n");
		cof_aux = new Cofactor(ddd_mgr,
		matrix->sys_det, PLUS, 1, 1);
		expr_aux->add_den_cofactor(cof_aux);
	    }
        }
    }
    num_exprs = ecount - 1;
    print_mesg("END OF BUILDING SYMBOLIC EXPRESSIONS");
}

/*----------------------------------------------------
Spec: perform numerical ac analysis based on 
explicit coefficient expressions.
-----------------------------------------------------*/
void
ACAnalysisManager::build_fcoeff_from_ddd()
{
    double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    double time2;
    double c_time;

    print_mesg("EXPLICIT NUMERICAL AC ANALYSIS ...");
    if(!matrix->index_mat)
        theCkt->build_mna();
    if(!matrix->sys_det)
        build_sys_ddd();
    if(!poly)
        build_expressions();

    if(!theFLabMap)
        fcoeff_build_expressions_from_ddd();
#ifdef _DEBUG
    time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    fcoeff_freq_sweep();
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"Evaluation time time: " << c_time <<endl;
    //graph_display();
#endif
    print_mesg("END OF NUMERICAL AC ANALYSIS");
}

/*----------------------------------------------------
Spec: perform numerical ac analysis based on 
explicit coefficient expressions. The fully
expanded expression is coming from the
simplified complex DDD sddd.
-----------------------------------------------------*/
void
ACAnalysisManager::build_fcoeff_from_sddd()
{
    double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    double time2;
    double c_time;

    print_mesg("BUILD FCOEFFICIENT LIST FROM SDDD");
    if(!matrix->index_mat)
        theCkt->build_mna();
    if(!matrix->sys_det)
        build_sys_ddd();
    if(!poly)
        build_expressions();
    if(!ddd_simplied)
        simplify_ddd();
    if(!theFLabMap)
        fcoeff_build_expressions_from_sddd();
    time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    fcoeff_freq_sweep();
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"Evaluation time time: " << c_time <<endl;
    //graph_display();
    print_mesg("END OF NUMERICAL AC ANALYSIS");
}

/*----------------------------------------------------
Spec: For each ExpList, we build two coefficient lists
for numerator and denominator respectively. In our 
current implementation, we assume the voltage gain
is the required output, so, numerator and 
denominator has the same number of symbols.
-----------------------------------------------------*/

void 
ACAnalysisManager::coeff_build_expressions()
{
    ExpList    *exp_aux = poly;
    Cofactor    *cof_aux;
    CoeffList    *sum_list, *new_list;
    int        nprod;
    double    c_time = 0.0, e_time = 0.0;
    double  time1, time2;
    time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = c_time + time2 - time1;


    if(!poly){    
        error_mesg(INT_ERROR,"No expressions exist.");
        return;
    }

    // initialization for coefficient construction.

	/* maximum # of symbols in a product terms is the size of matrix */
    nprod = matrix->size;

    theLabMap = new LabelMap(matrix->num_elem);

   	HasherTree *ht    = new HasherTree(FALSE);

    for(; exp_aux; exp_aux = exp_aux->next){
        // create the numerator and denomator coefficient list

        /* build the coefficient for numerator */
        sum_list = NULL;
        cout <<"\nBuilding coefficient for numerator"<<endl;
        for(cof_aux = exp_aux->num_list; cof_aux; cof_aux = cof_aux->next){
            time1 = 1.0 * clock() / CLOCKS_PER_SEC;
            cof_aux->coeff_list =
            ddd_mgr->generate_all_coeffs(cof_aux->get_cddd(), nprod);
            time2 = 1.0 * clock() / CLOCKS_PER_SEC;
            c_time = c_time + time2 - time1;
            //cout <<"\nStatisitic for numerator coefficient list"<<endl;
            if(cof_aux->coeff_list){
                //cof_aux->coeff_list->statistic(cout);
                //time1 = 1.0 * clock() / CLOCKS_PER_SEC;
                cof_aux->coeff_list->calc_coeff_list(ht);
                //time2 = 1.0 * clock() / CLOCKS_PER_SEC;
                //e_time = e_time + time2 - time1;
            }
        }

        /* build the coefficient for denominator */
        cout <<"\nBuilding coefficient for denominator"<<endl;
        for(cof_aux = exp_aux->den_list; cof_aux; cof_aux = cof_aux->next){
            time1 = 1.0 * clock() / CLOCKS_PER_SEC;
            cof_aux->coeff_list =
            ddd_mgr->generate_all_coeffs(cof_aux->get_cddd(),nprod);
            time2 = 1.0 * clock() / CLOCKS_PER_SEC;
            c_time = c_time + time2 - time1;
            //cout <<"\nStatisitic for denominator coefficient list"<<endl;
            if(cof_aux->coeff_list){
                //cof_aux->coeff_list->statistic(cout);
                //time1 = 1.0 * clock() / CLOCKS_PER_SEC;
                cof_aux->coeff_list->calc_coeff_list(ht);
                //time2 = 1.0 * clock() / CLOCKS_PER_SEC;
                //e_time = e_time + time2 - time1;
            }
        }
    }
	delete ht;
    cout <<"total constrction time: "<<c_time<<endl;
    cout <<"total coeff evaluation time: "<<e_time<<endl;
}


/*----------------------------------------------------
Spec: For each ExpList, we build two 
fully expanded coefficient lists
for numerator and denominator respectively. In our 
current implementation, we assume the voltage gain
is the required output, so, numerator and 
denominator has the same product term length.
-----------------------------------------------------*/

void 
ACAnalysisManager::fcoeff_build_expressions_from_ddd()
{
    ExpList  *exp_aux = poly;
    Cofactor *cof_aux;
    CoeffList *sum_list, *new_list;
    int    nprod;
    double    c_time = 0.0, e_time = 0.0;
    double  time1, time2;
    time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = c_time + time2 - time1;
    int max_branch = 0;

    if(!poly){    
        error_mesg(INT_ERROR,"No expressions exist.");
        return;
    }

    // initialization for coefficient construction.

	/* maximum # of symbols in a product terms is the size of matrix */
    nprod = matrix->size;

    max_branch = matrix->max_branch_num();
    printf("Max branch number: %d\n",max_branch);
    theFLabMap = new FLabelMap(matrix->num_elem,max_branch);

    HasherTree *ht    = new HasherTree(FALSE);

    for(; exp_aux; exp_aux = exp_aux->next){
        // create the numerator and denomator coefficient list

        /* build the coefficient for numerator */
        sum_list = NULL;
        cout <<"\nBuilding coefficient for numerator"<<endl;
        for(cof_aux = exp_aux->num_list; cof_aux; cof_aux = cof_aux->next){
            time1 = 1.0 * clock() / CLOCKS_PER_SEC;
            cof_aux->fcoeff_list =
            ddd_mgr->generate_all_fcoeffs(cof_aux->get_sddd(), 
                        nprod,NUM);
            time2 = 1.0 * clock() / CLOCKS_PER_SEC;
            c_time = c_time + time2 - time1;
            cout <<"\nStatisitic for numerator coefficient list"<<endl;
            if(cof_aux->fcoeff_list){
                cof_aux->fcoeff_list->statistic(cout);
                time1 = 1.0 * clock() / CLOCKS_PER_SEC;
                cof_aux->fcoeff_list->calc_fcoeff_list(ht);
                time2 = 1.0 * clock() / CLOCKS_PER_SEC;
                e_time = e_time + time2 - time1;
            }
        }

        /* build the coefficient for denominator */
        cout <<"\nBuilding coefficient for denominator"<<endl;
        for(cof_aux = exp_aux->den_list; cof_aux; cof_aux = cof_aux->next){
            time1 = 1.0 * clock() / CLOCKS_PER_SEC;
            cof_aux->fcoeff_list =
            ddd_mgr->generate_all_fcoeffs(cof_aux->get_sddd(),
                    nprod,DEN);
            time2 = 1.0 * clock() / CLOCKS_PER_SEC;
            c_time = c_time + time2 - time1;
            cout <<"\nStatisitic for denominator coefficient list"<<endl;
            if(cof_aux->fcoeff_list){
                cof_aux->fcoeff_list->statistic(cout);
                time1 = 1.0 * clock() / CLOCKS_PER_SEC;
                cof_aux->fcoeff_list->calc_fcoeff_list(ht);
                time2 = 1.0 * clock() / CLOCKS_PER_SEC;
                e_time = e_time + time2 - time1;
            }
        }
    }

	delete ht;

    cout <<"total constrction time: "<<c_time<<endl;
    cout <<"total coeff evaluation time: "<<e_time<<endl;
}

/*----------------------------------------------------
Spec: For each ExpList, we build two 
fully expanded coefficient lists
for numerator and denominator respectively. 

The DDD we use is sddd instead of cdd. 

In our current implementation, we assume the voltage gain
is the required output, so, numerator and 
denominator has the same product term length.
-----------------------------------------------------*/

void 
ACAnalysisManager::fcoeff_build_expressions_from_sddd()
{
    ExpList    *exp_aux = poly;
    Cofactor *cof_aux;
    CoeffList *sum_list, *new_list;
    int    nprod;
    double    c_time = 0.0, e_time = 0.0;
    double  time1, time2;
    time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = c_time + time2 - time1;
    int max_branch = 0;

    if(!poly){    
        error_mesg(INT_ERROR,"No expressions exist.");
        return;
    }

    // initialization for coefficient construction.

    nprod = matrix->size;

    max_branch = matrix->max_branch_num();
    printf("Max branch number: %d\n",max_branch);
    theFLabMap = new FLabelMap(matrix->num_elem,max_branch);

	HasherTree *ht    = new HasherTree(FALSE);

    for(; exp_aux; exp_aux = exp_aux->next){
        // create the numerator and denomator coefficient list

        /* build the coefficient for numerator */
        sum_list = NULL;
        cout <<"\nBuilding coefficient for numerator"<<endl;
        for(cof_aux = exp_aux->num_list; cof_aux; cof_aux = cof_aux->next){
            time1 = 1.0 * clock() / CLOCKS_PER_SEC;
            cof_aux->fcoeff_list =
            ddd_mgr->generate_all_fcoeffs(cof_aux->get_sddd(), 
                        nprod,NUM);
            time2 = 1.0 * clock() / CLOCKS_PER_SEC;
            c_time = c_time + time2 - time1;
            cout <<"\nStatisitic for numerator coefficient list"<<endl;
            if(cof_aux->fcoeff_list){
                //cof_aux->fcoeff_list->statistic(cout);
                time1 = 1.0 * clock() / CLOCKS_PER_SEC;
                cof_aux->fcoeff_list->calc_fcoeff_list(ht);
                time2 = 1.0 * clock() / CLOCKS_PER_SEC;
                e_time = e_time + time2 - time1;
            }
        }

        /* build the coefficient for denominator */
        cout <<"\nBuilding coefficient for denominator"<<endl;
        for(cof_aux = exp_aux->den_list; cof_aux; cof_aux = cof_aux->next){
            time1 = 1.0 * clock() / CLOCKS_PER_SEC;
            cof_aux->fcoeff_list =
            ddd_mgr->generate_all_fcoeffs(cof_aux->get_sddd(),
                        nprod,DEN);
            time2 = 1.0 * clock() / CLOCKS_PER_SEC;
            c_time = c_time + time2 - time1;
            cout <<"\nStatisitic for denominator coefficient list"<<endl;
            if(cof_aux->fcoeff_list){
                cof_aux->fcoeff_list->statistic(cout);
                time1 = 1.0 * clock() / CLOCKS_PER_SEC;
                cof_aux->fcoeff_list->calc_fcoeff_list(ht);
                time2 = 1.0 * clock() / CLOCKS_PER_SEC;
                e_time = e_time + time2 - time1;
            }
        }
    }
	delete ht;
    cout <<"total constrction time: "<<c_time<<endl;
    cout <<"total coeff evaluation time: "<<e_time<<endl;
}

/*----------------------------------------------------
Spec: build the ddd for the whole MNA matrix
Note: row and col is the index of nodes of 
input circuit so, we do not need explicit transformation.
-----------------------------------------------------*/
DDDnode *
ACAnalysisManager::build_cofactor_ddd( int row, int col )
{
    int ind_row, ind_col;
    int index;
    DDDnode    *ddd_aux;

    if(!matrix){
        error_mesg(INT_ERROR,"No MNA matrix");
        return NULL;
    }
    ind_row = row;
    ind_col = col;

    sprintf(_buf,"BUILDING COFACTOR (%d %d) ...", row,col);
    print_mesg(_buf);

    if(ind_row != -1 && ind_col != -1){
        if((index = matrix->get_label(ind_row, ind_col)) != 0){
            print_mesg("COFACTOR IS PRESENT.");
            ddd_aux = ddd_mgr->Subset1(matrix->sys_det, index);
            matrix->index_mat->ddd_statistic(ddd_aux);
            //print_mesg("END BUILDING COFACTOR.\n");
            return ddd_aux;
        }
        else{
            print_mesg("MATRIX EXPANSION IS PERFORMED...");
            //matrix->index_mat->reset_rowcol();
            matrix->index_mat->initial_rowcol();
            ddd_aux = 
                matrix->index_mat->matrix_recursive_expR(ind_row, ind_col);
            if(ddd_aux)
                matrix->index_mat->ddd_statistic(ddd_aux);
            //print_mesg("END BUILDING COFACTOR.\n");
            return ddd_aux;
        }
    }
    else {
        print_error(FAT_ERROR,"Invalid row and col number(real)");
        return NULL;
    }
}

/*----------------------------------------------------
Spec: build the ddd for the whole MNA matrix
-----------------------------------------------------*/
void
ACAnalysisManager::build_sys_ddd()
{
    if(!matrix){
        error_mesg(INT_ERROR,"Invalid MNA matrix");
        return;
    }

    print_mesg("BUILDING SYSTEM DDD ...");
    matrix->build_sys_ddd();
    //print_mesg("END OF BUILDING SYSTEM DDD");
        
}

/*
**    Add a new expression into the linked list
**  with the order of adding
*/  
    
void
ACAnalysisManager::add_exprs( ExpList *expr)
{   
    ExpList    *laux = poly; 
    
    if(!poly)
        poly = expr; 
    else{   
        while(laux->Next()) laux = laux->Next();
        laux->Next(expr);
    }
}   
