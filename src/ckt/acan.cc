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
 *    $RCSfile: acan.cc,v $
 *    $Revision: 1.11 $
 *    $Date: 2002/11/21 02:40:15 $
 */

#include <math.h>
#include "circuits.h"
#include "control.h"
#include "acan.h"
#include "mxexp.h"
#include "mna_form.h"
#include "graph_out.h"
#include "approx.h"

#define MAX_PTS_PER_DECADE 10000
//#define _DEBUG

/* static globals variable definitions */
IOList * ACAnalysisManager::inputs;
IOList * ACAnalysisManager::outputs;
int      ACAnalysisManager::input_num;
int      ACAnalysisManager::output_num;

    
//////////////////////////////////////////////////////
// the member function definition of IOList
//////////////////////////////////////////////////////
/* constructor and distractor */
IOList::IOList
( 
	int _n1, 
	int _n2, 
	DeviceType _type,
	double _value, 
	OutFormat _format, 
	int _depfreq 
)
{
    node1     = _n1;
    node2     = _n2;
    type      = _type;
    format    = _format;
    value     = _value;
	dep_freq  = _depfreq;
    next      = NULL;
}

IOList::~IOList()
{
    if(next)
	{
        delete next;
	}
}
//////////////////////////////////////////////////////
// the member function definition of ACAnalysis
//////////////////////////////////////////////////////

/*----------------------------------------------------
Spec: constructor and distroctor
-----------------------------------------------------*/
ACAnalysis::ACAnalysis
(
	VariationType _type, 
    int _np, 
	double _fstart, 
	double _fstop
)
{
    var_type = _type; 
    np = _np;  
    fstart= _fstart;
    fstop = _fstop;
    mag_err = 0.0;
    phs_err = 0.0;
    if(!check_params())
	{
        cal_samp_pts();
        cal_freq_pts();
    }
}

ACAnalysis::~ACAnalysis()
{
    if(freq_pts)
	{
        delete [] freq_pts;
	}
}


/*----------------------------------------------------
    FuncName: check_params
    Spec: check the given AC parameters to see 
          the if it is correct or not.
 -----------------------------------------------------
    Inputs:
    Outputs: boolean value indicate the

    Global Variblas used:
----------------------------------------------------*/

int ACAnalysis::check_params()
{
	if (fstart <= 0)
	{
		error_mesg(FAT_ERROR,"Invalid starting frequency");
		return -1;
	}
	if (fstop <= fstart)
	{
		error_mesg(FAT_ERROR,"Invalid stop frequency ( < fstart)");
		return -1;
	}  
	if (np <= 0) 
	{
		error_mesg(FAT_ERROR,"Invalid point per variation");
		return -1;
	}
	if (np > MAX_PTS_PER_DECADE)
	{
		error_mesg(FAT_ERROR,"The number per variation is too large!" );
		return -1;
	}

	return 0;
}


/*----------------------------------------------------
Spec: calculate the real sampling points needed
-----------------------------------------------------*/
void ACAnalysis::cal_samp_pts()
{

    if( var_type == vDEC )
	{
    	num_pts = (int) (np * log10(0.99 * fstop / fstart) + 2);
	}
    else if( var_type == vOCT )
	{
    	num_pts = (int) (np * log(fstop/fstart)/log(2));
	}
    else if( var_type == vLIN )
	{
    	num_pts = (int) np;
	}
    else
	{
        print_error(INT_ERROR,"Invalid variation type");
    }
}


/*----------------------------------------------------
Spec: calculate all the frequency points needed
-----------------------------------------------------*/
void
ACAnalysis::cal_freq_pts()
{
    int i; 
	double step;

    if(!num_pts)
	{
        error_mesg(INT_ERROR,"number of frequency points have not given!");
        return;
    }

    freq_pts = new double[num_pts];

    switch(var_type)
	{
	case vDEC:
		for (i = 0; i < num_pts; i++)
		{
			freq_pts[i] = fstart * exp(log(10.0) * (1.0/np) * i );
		}
		freq_pts[num_pts-1] = fstop;
		break;
    
	case vOCT:
		for (i = 0; i < num_pts; i++)
		{
			freq_pts[i] = fstart*pow(2, (double)i/(double)np);
		}
		freq_pts[num_pts-1] = fstop;
		break;
        
	case vLIN:
		step = (fstop-fstart)/(np + 1);
		for(i = 0; i < num_pts; i++)
		{
			freq_pts[i] = fstart + step * i;
		}
		freq_pts[num_pts-1] = fstop;
		break;

	default:
		break;
	}
}


//////////////////////////////////////////////////////
// the member function definition of Cofactor
//////////////////////////////////////////////////////
/*
**    Note that, _row and _col are the index number of node.
*/

// The constructor
Cofactor::Cofactor
(
	DDDmanager *_ddd_mgr, 
	DDDnode *_ddd, 
	int _sign, 
    int _row, 
	int _col
)
{
    ddd_mgr = _ddd_mgr;
    cddd = _ddd;
    sddd = _ddd;
    symb_name = NULL;
    sign = _sign;
    row = _row;
    col = _col;
    freq = -1e20;
    coeff_list = NULL;
    fcoeff_list = NULL;
    sfcoeff_list = NULL;
    next = NULL;

    int isign = (row + col) % 2;
 
	if(isign == 0)
	{
		isign = 1;
	}
	else
	{
		isign = -1;
	}
 
    osign = sign = sign * isign;

    // initialize the cddd for numerical evalution
    // initial_calc();
#ifndef COLLECT_COEFF_DATA
    if(cddd)
	{
        _ddd_mgr->Keep(cddd);
	}
    else
	{
        print_mesg("\007No ddd is found for the cofactor!!!\n");
	}
#endif

}


Cofactor::~Cofactor()
{
    //delete cddd;
    //if(next)
    //    delete next;
    if(coeff_list)
	{
        delete coeff_list;
	}
    if(fcoeff_list)
	{
        delete fcoeff_list;
	}
    if(sfcoeff_list)
	{
        delete sfcoeff_list;
	}

    if(symb_name)
	{
		delete symb_name;
	}
}


/*----------------------------------------------------
    FuncName: initial_calc
    Spec: initialize the calculation flag
          and allocate the complex value
          for each DDD node.
 -----------------------------------------------------
    Inputs: DDD
    Outputs: initialized DDD

    Global Variblas used:
----------------------------------------------------*/
void Cofactor::initial_calc
(
	HasherTree *prec_ht
)
{
    if(!cddd)
	{
        sprintf(_buf,"Cofactor(%d %d) = 0.0",row,col);
        print_warn(_buf);
        return;
    }
	
	ddd_mgr->PreCalculationR(cddd, prec_ht);
}

/* initilaize the simplified ddd for numerical evaluation */
void Cofactor::sddd_init
(
	HasherTree *prec_ht
)
{
    if(!sddd)
	{
        sprintf(_buf,"sddd = NULL in cofactor(%d %d)",row,col);
        print_warn(_buf);
        return;
    }
	
    if(!sddd->GetValueP())
	{
        ddd_mgr->PreCalculationR(sddd, prec_ht);
        //ddd_mgr->PrintSets(sddd);
        //ddd_mgr->statistic(sddd);
    }
}
    


/*----------------------------------------------------
    FuncName: evaluate
    Spec:
    calculate the numerical value of the determinant
    the given DDD represents
 -----------------------------------------------------
    Inputs: DDDnode * ddd, double freq
    Outputs: numerical value of the derterminant

    Global Varibles used:
----------------------------------------------------*/

complex Cofactor::evaluate
(
	double _freq, 
	HasherTree *prec_ht,
	HasherTree *ht, 
	bool use_simp // use simplified ddd or not
)
{
    //int cur_caled = 0;


    if(use_simp)
	{
        if(!sddd)
		{
            return 0.0;
		}
        sddd_init(prec_ht);
       
        value = ddd_mgr->GetDetermValueR(sddd, _freq, ht);		

        if(sign < 0)
		{
            value = -1.0 * value;
		}
        return value;
    }

    if(!cddd)
	{
        return 0.0;
    }

    initial_calc(prec_ht);

    if(_freq == freq )
	{
        return value;
	}
    freq = _freq;

    value = ddd_mgr->GetDetermValueR(cddd, _freq, ht);

	// to dump the complex DDD structures.
	//ddd_mgr->PrintDDDTree(cddd);
	
    if(osign < 0)
	{
        value = -1.0 * value;
	}

	return value;
 
}


complex Cofactor::coeff_evaluate
(
	double _freq
)
{
    if(!cddd || !coeff_list)
	{
        return 0.0;
    }
    if(_freq == freq )
	{
        return value;
	}

    freq = _freq;

	value = coeff_list->evaluate(_freq);

    if(sign < 0)
	{
        value = -1.0 * value;
	}

	return value;
 
}


complex Cofactor::fcoeff_evaluate
(
	double _freq,
	bool use_simp
)
{
    if(use_simp)
	{
        if(!cddd || !sfcoeff_list)
		{
            error_mesg(INT_ERROR,"No cddd or sfcoeff_list exits");
            return 0.0;
        }
        value = sfcoeff_list->evaluate(_freq);
    }
    else
	{

        if(!cddd || !fcoeff_list)
            return 0.0;

        value = fcoeff_list->evaluate(_freq);
    }

    if(sign < 0)
	{
        value = -1.0 * value;
	}

	return value;
}


/*
** Print out all dominanted terms after simplification steps
*/
void Cofactor::print_fcoeff_list
(
	ostream & out
)
{
	if(!sfcoeff_list)
	{
		//if(!fcoeff_list){
		out<<"No simplified terms found"<<endl;
		return;
	}

	if(sign>0)
	{
		out<<"(+) * ["<<endl;
	}
	else
	{
		out<<"(-) * ["<<endl;
	}
  
	sfcoeff_list->print_fcoeff_list(out);
	
    //fcoeff_list->print_fcoeff_list(out);
	out<<"]"<<endl;
}

void
Cofactor::fcoeff_print_transfer_func
(
	ostream & out
)
{
	if(!cddd || !fcoeff_list)
	{
        return;
    }
   
	fcoeff_list->print_transfer_func(out, sign);

	return;
 
}
//////////////////////////////////////////////////////
// the member function definition of ExpList
//////////////////////////////////////////////////////

/*----------------------------------------------------
Spec: constructor and destructor of ExpList
-----------------------------------------------------*/
ExpList::ExpList
(
	IOList *_input, 
	int _num_pts, 
	int _index, 
	double _coeff
)
{
    input = _input;
    num_pts = _num_pts;
    coeff = _coeff;
    num_val = new complex[num_pts];
    den_val = new complex[num_pts];
    num_list = NULL;
    den_list = NULL;
    index = _index;
    next = NULL;
}

ExpList::~ExpList()
{
    delete [] num_val;
    delete [] den_val;
    if(num_list)
	{
		delete num_list;
	}
    if(den_list)
	{
		delete den_list;
	}
}


/*----------------------------------------------------
Spec: add cofactor into the num or den linked lists
-----------------------------------------------------*/
void ExpList::add_num_cofactor
(
	Cofactor *cofactor
)
{
    if(!cofactor)
	{
        return;
	}
    cofactor->Next(num_list);
    num_list = cofactor;
}    

void ExpList::add_den_cofactor
(
	Cofactor *cofactor
)
{
    if(!cofactor)
	{
        return;
	}
    cofactor->Next(den_list);
    den_list = cofactor;
}

/*----------------------------------------------------
Spec: calculated the numerical value of the expressionlist
-----------------------------------------------------*/
complex ExpList::evaluate
(
	double _freq, 
	int findex, 
	MNAMatrix *matrix,
	HasherTree *prec_ht,
	HasherTree *ht, 
	bool use_simp,       // use simplifed ddd (sddd)
	bool enable_bch_elim // enable branch elimination
)
{
    Cofactor *cofact_aux = num_list;
    complex  num_value = 0.0, den_value = 0.0;
	complex  result;

    if(!num_list || !den_list)
	{
        error_mesg(INT_ERROR,"Invalid num or den cofactor list");
        return complex(0.0, 0.0);
    }
    if(enable_bch_elim)
	{
        matrix->calc_value(NUM);
	}
    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
        num_value = num_value + 
			cofact_aux->evaluate( _freq, prec_ht, ht, use_simp );
	}
    num_val[findex] = num_value;

    if(enable_bch_elim)
	{
        matrix->calc_value(DEN);
	}
    cofact_aux = den_list;
    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
        den_value = den_value + 
			cofact_aux->evaluate( _freq, prec_ht, ht , use_simp);
	}
    
    den_val[findex] = den_value;	
	result = coeff * (num_value/den_value);	
	return result; 
}

   
/*----------------------------------------------------
Spec: calculated the numerical value of coefficient lists;
-----------------------------------------------------*/
complex ExpList::coeff_evaluate
( 
	double _freq
)
{
    Cofactor *cofact_aux = num_list;
    complex  num_value = 0.0, den_value = 0.0;

    if(!num_list || !den_list)
	{
    	error_mesg(INT_ERROR, "Invalid num or den coefficient list");
		return complex(0.0, 0.0);
    }
    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
    	num_value = num_value + cofact_aux->coeff_evaluate( _freq);
	}

    cofact_aux = den_list;
    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
    	den_value = den_value + cofact_aux->coeff_evaluate( _freq);
	}

    return coeff * (num_value/den_value);

}


/*----------------------------------------------------
Spec: calculated the numerical value of full coefficient lists;
-----------------------------------------------------*/
complex ExpList::fcoeff_evaluate
( 
	double _freq,
	bool use_simp
)
{
    Cofactor *cofact_aux = num_list;
    complex  num_value = 0.0, den_value = 0.0;

    if(!num_list || !den_list)
	{
		error_mesg(INT_ERROR, "Invalid num or den coefficient list");
		return complex(0.0, 0.0);
    }
    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
    	num_value = num_value + cofact_aux->fcoeff_evaluate( _freq, use_simp);
	}

    cofact_aux = den_list;
    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
    	den_value = den_value + cofact_aux->fcoeff_evaluate( _freq, use_simp);
	}

    return coeff * (num_value/den_value);
}


// reallocate the space for exact value of numerator and demoninator
void ExpList::new_freq_number
( 
	int _num_pts
)
{
    delete [] num_val;
    delete [] den_val;
    num_val = new complex[_num_pts];
    den_val = new complex[_num_pts];
    num_pts = _num_pts;
}


void ExpList::print_transfer_func
(
	ostream & out
)
{	
	if(!num_list || !den_list)
	{
		error_mesg(INT_ERROR, "Invalid num or den coefficient list");
		return;		
    }
	
	Cofactor *cofact_aux = num_list;
    for(int i=0; cofact_aux ; i++, cofact_aux = cofact_aux->Next())
	{
		out <<"N" << i << " = [ ";
    	cofact_aux->fcoeff_print_transfer_func(out);
		out << " ];" << endl;	
	}
	
    cofact_aux = den_list;
    for(int j = 0; cofact_aux ; j++, cofact_aux = cofact_aux->Next())
	{
		out <<"D"<< j << " = [ ";
    	cofact_aux->fcoeff_print_transfer_func(out);
		out << " ];" << endl;
	}	
}


//////////////////////////////////////////////////////
// the member function definition of ACAnalysisManager
//////////////////////////////////////////////////////

/*----------------------------------------------------
Spec: constructor and destructor
-----------------------------------------------------*/
ACAnalysisManager::ACAnalysisManager
(
	MNAMatrix *_matrix, 
	DDDmanager *_ddd_mgr
)
{
    ddd_mgr     = _ddd_mgr;
    command     = NULL;
    matrix      = _matrix;
    num_pts     = 0; 
    out_value   = NULL;
    exact_value = NULL;
    poly        = NULL;
    graph_out   = NULL;
    inputs      = NULL;
    outputs     = NULL;
    input_num   = 0;
    output_num  = 0;
    ddd_simplied  = 0;
    fcoeff_simped = 0;
    num_exprs   = 0;
}


ACAnalysisManager::~ACAnalysisManager()
{ 

    if( command )
	{
        delete command;
	}

    if(out_value)
	{
        delete [] out_value;
	}

    if(exact_value)
	{
        delete [] exact_value;
	}

    if(poly)
	{
        delete poly;
	}

    if(graph_out)
	{
        delete graph_out;
	}

    if(inputs)
	{
        delete inputs;
		// inputs is static member
		inputs = NULL;
    }
    
    if(outputs)
	{
        delete outputs;
		// outputs is a static member
		outputs = NULL;
    }
}



/*----------------------------------------------------
Spec: do the frequency swapping operation for
    whole expressions.
-----------------------------------------------------*/
void ACAnalysisManager::freq_sweep
(
	bool use_simp,
	bool enable_bch_elim
)
{
    double *freq = command->freq_pts;

    if(!num_pts)
	{
        error_mesg(INT_ERROR,"Invalid num_pts");
        return;
    }

    if(!poly)
	{
        error_mesg(INT_ERROR,"No symbolic expressions generated");
        return;
    }

    print_mesg("FREQUENCY SWEEPING ...");

    if(!out_value)
	{
        out_value = new complex[num_pts];
	}
    if(!exact_value)
	{
        exact_value = new complex[num_pts];
	}

    for (int i = 0; i < num_pts; i++) 
	{
        /* used for the DDD evaluation in top level circuit */
		HasherTree *prec_ht    = new HasherTree(FALSE);
		HasherTree *ht    = new HasherTree(FALSE);

        /* used for the DDD evaluation in subcircuits */
        theCkt->sub_init_ht();

    	if(use_simp)
		{
			out_value[i] = evaluate
				(
					freq[i], 
					i,
					prec_ht,
					ht, 
					use_simp, 
					enable_bch_elim
				);
		}
        else
		{
			matrix->calc_value();
            out_value[i] = exact_value[i] =  evaluate
				(
					freq[i], 
					i,
					prec_ht,
					ht, 
					use_simp,
					enable_bch_elim
				);
        }
		delete ht;
		delete prec_ht;		
    }

    print_mesg("END OF FREQUENCY SWEEPING ");
}

 
complex ACAnalysisManager::evaluate
( 
	double freq , 
	int freq_index, 
	HasherTree *prec_ht,
	HasherTree *ht, 
	bool use_simp,
	bool enable_bch_elim
)
{

    ExpList * poly_list = poly;
    complex    cvalue = 0.0;

    if(!poly_list)
	{
        error_mesg(INT_ERROR,"Now symbolic polynomial expressions derived");
        return cvalue;
    }
    for(; poly_list ; poly_list = poly_list->next)
	{
        cvalue += poly_list->evaluate
			(
				freq,
				freq_index,
				matrix, 
				prec_ht,
				ht, 
				use_simp,
				enable_bch_elim
			);
    }

    return cvalue;
}


/*----------------------------------------------------
Spec: Free the space used by previous ac analysis 
-----------------------------------------------------*/
void ACAnalysisManager::free_command()
{
    if(out_value)
	{
        delete [] out_value;
        out_value = NULL;
    }
    if(graph_out)
	{
        delete graph_out;
        graph_out = NULL;
    }
    if(exact_value)
	{
        delete [] exact_value;
        exact_value = NULL;
    }
}


/*----------------------------------------------------
Spec: add new ac analysis command and assign new "num_pts"
-----------------------------------------------------*/

void ACAnalysisManager::add_command
( 
	ACAnalysis *_command
)
{
    if(!_command)
	{
        return;
	}
    if(command) // free previous command space
	{
        delete command;
	}
    command = _command;
    num_pts = _command->num_pts;
    // we assume just one polynomial exists
    if(poly)
	{
        poly->new_freq_number(num_pts);
	}
}


/*----------------------------------------------------
Spec:
Add new command and free the space by previous ac command
rebuild the symbolic expression
-----------------------------------------------------*/

void ACAnalysisManager::new_command
(
	ACAnalysis *_command
)
{
    if(!_command)
	{
        return;
	}
    free_command();
    add_command(_command);
}


/*----------------------------------------------------
Spec: perform numerical ac analysis
-----------------------------------------------------*/
void ACAnalysisManager::ac_analysis()
{
    double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    double time2;
    double c_time;

    print_mesg("IMPLICIT NUMERICAL AC ANALYSIS ...");
    if(!matrix->index_mat)
	{
        theCkt->build_mna();
	}
    if(!matrix->sys_det)
	{
        build_sys_ddd();
	}
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"DDD construction time: " << c_time <<endl;
   
	if(!poly)
	{
        build_expressions();
	}

    time1 = time2;
    freq_sweep(FALSE, FALSE);
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"Evaluation time time: " << c_time <<endl;
    //graph_display();
    print_mesg("END OF NUMERICAL AC ANALYSIS");
}


/*----------------------------------------------------
Spec: perform numerical ac analysis based on 
explicit coefficient expressions.
-----------------------------------------------------*/
void ACAnalysisManager::coeff_ac_analysis()
{
    double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    double time2;
    double c_time;

    print_mesg("EXPLICIT NUMERICAL AC ANALYSIS ...");
    if(!matrix->index_mat)
	{
        theCkt->build_mna();
	}
    if(!matrix->sys_det)
	{
        build_sys_ddd();
	}
    if(!poly)
	{
        build_expressions();
	}

    if(!theLabMap)
	{
        coeff_build_expressions();
	}

    time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    coeff_freq_sweep();
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"Evaluation time time: " << c_time <<endl;
    //graph_display();
    print_mesg("END OF NUMERICAL AC ANALYSIS");
}


complex ACAnalysisManager::coeff_evaluate
( 
	double _freq
)
{

    ExpList * poly_list = poly;
    complex cvalue = 0.0;

    if(!poly_list)
	{
        error_mesg(INT_ERROR,"Now symbolic polynomial expressions derived");
        return cvalue;
    }

    for(; poly_list ; poly_list = poly_list->next)
	{
        cvalue += poly_list->coeff_evaluate( _freq );
    }

    return cvalue;

}

complex ACAnalysisManager::fcoeff_evaluate
( 
	double _freq,
	bool use_simp
)
{

    ExpList * poly_list = poly;
    complex cvalue = 0.0;

    if(!poly_list){
        error_mesg(INT_ERROR,"Now symbolic polynomial expressions derived");
        return cvalue;
    }

    for(; poly_list ; poly_list = poly_list->next)
	{
        cvalue += poly_list->fcoeff_evaluate( _freq, use_simp);
    }
	
    return cvalue;

}

void ACAnalysisManager::coeff_freq_sweep()
{
    double *freq = command->freq_pts;

    if(!num_pts)
	{
        error_mesg(INT_ERROR,"Invalid num_pts");
        return;
    }

    if(!poly)
	{
        error_mesg(INT_ERROR,"No expressions generated");
        return;
    }

    print_mesg("FREQUENCY SWEEPING ...");
    if(!out_value)
	{
        out_value = new complex[num_pts];
	}

    for (int i = 0; i < num_pts; i++)
	{
        out_value[i] = coeff_evaluate(freq[i]);
	}

    print_mesg("END OF FREQUENCY SWEEPING ");

}


void ACAnalysisManager::fcoeff_freq_sweep
(
	bool use_simp
)
{
    double *freq = command->freq_pts;

    if(!num_pts)
	{
        error_mesg(INT_ERROR,"Invalid num_pts");
        return;
    }

    if(!poly)
	{
        error_mesg(INT_ERROR,"No expressions generated");
        return;
    }

    print_mesg("FREQUENCY SWEEPING ...");

    if(!out_value)
	{
        out_value = new complex[num_pts];
	}

    for (int i = 0; i < num_pts; i++)
	{
        out_value[i] = fcoeff_evaluate(freq[i], use_simp);
	}

    print_mesg("END OF FREQUENCY SWEEPING ");
}


/*----------------------------------------------------
Spec: text oriented output for further use by other
    CAD system
-----------------------------------------------------*/
void ACAnalysisManager::text_output(char *_filename)
{ ; }

/*----------------------------------------------------
Spec: add the excitation node(input) 
-----------------------------------------------------*/
void ACAnalysisManager::add_input( IOList *_inlist )
{
    if(!_inlist)
	{
        return;
	}

    _inlist->Next(inputs);
    inputs = _inlist;
    input_num++;
}

/*----------------------------------------------------
Spec: add the  node to be sampled(output) 
      Note: we only allow one output now.
-----------------------------------------------------*/
void ACAnalysisManager::add_output
( 
	IOList *_outlist 
)
{
    if(!_outlist)
	{
        return;
	}
    outputs = _outlist;
    output_num = 1;
    /*
	  _outlist->Next(outputs);
	  outputs = _outlist;
    */
}


    
/*
** Print out all dominanted terms after simplification steps
*/
void ACAnalysisManager::print_fcoeff_list
(
	ostream & out
)
{
	if(!poly)
	{
		error_mesg(INT_ERROR,"Not expression exist, abort!");
		return;
	}

	if(!theTopoList)
	{
		theTopoList = new DDDstack;
	}

	out<<"Simplified Symbolic Expression"<<endl;
	out<<"numerator:"<<endl;
	poly->num_list->print_fcoeff_list(out);
	out<<"-----------------------------"<<endl;
	out<<"denominator:"<<endl;
	poly->den_list->print_fcoeff_list(out);
	delete theTopoList;
	theTopoList = NULL;
}

/* perform the frequency sweeping according to specific requirement */
void ACAnalysisManager::new_freq_sweep
(
	char *_spec
)
{
    char *spec = ToLower(_spec);

	if(!poly)
	{
		error_mesg(INT_ERROR,"Not expression exist, abort!");
		return;
	}

    if(!spec)
	{
		bool useSimpExp = 0;
		bool enableBranchElim = 0;
        freq_sweep(useSimpExp, enableBranchElim);
	}
    else if(!strcmp(spec,"ddd"))
	{
        // for orginal complex DDD
		bool useSimpExp = 0;
		bool enableBranchElim = 0;
        freq_sweep(useSimpExp, enableBranchElim);
	}
    else if(!strcmp(spec,"sddd"))
	{
        // for simplified orginal complex DDD
        
        bool useSimpExp = 1;
		bool enableBranchElim = 1;
        freq_sweep(useSimpExp, enableBranchElim);
       
    }
    else if(!strcmp(spec,"coeff"))
	{
		bool useSimpExp = 0;
        fcoeff_freq_sweep(useSimpExp);
    }
    else if(!strcmp(spec,"scoeff"))
	{
        bool useSimpExp = 1;
        fcoeff_freq_sweep(useSimpExp);
    }
    else
	{
        sprintf(_buf,"Unknown spec: %s",spec);
        error_mesg(IO_ERROR,_buf);
    }

}

/* caluculate all numerical value of coefficients */
void 
ACAnalysisManager::calc_fcoeff_list_value()

{
	ExpList * poly_list;

	for(poly_list = poly; poly_list; poly_list = poly_list->next)
	{
		poly_list->calc_fcoeff_list();
	}
}


/* caluculate all numerical value of simplied coefficients */
void 
ACAnalysisManager::calc_sfcoeff_list_value()
{
	ExpList * poly_list;
	for(poly_list = poly; poly_list; poly_list = poly_list->next)
	{
		poly_list->calc_sfcoeff_list();
	}
}

/* dump the numerically s-expanded transfer function. We assume the
   the s-expanded DDDs are obtained. The output format is matlab ready
   where the D[0] is the largest coefficient of power of s in
   polynomial D.
 */

void
ACAnalysisManager::transfer_func_cmd
(
	ostream& out
)
{
	ExpList * poly_list = poly;

	for(poly_list = poly; poly_list; poly_list = poly_list->next)
	{
		poly_list->print_transfer_func(out);
	}
	
}
