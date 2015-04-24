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
 *    $RCSfile: noisean.cc,v $
 *    $Revision: 1.7 $
 *    $Date: 2002/11/21 02:40:15 $
 */


/*
**    rountins to obtain the noise spectral density 
*/

#include <math.h>
#include "circuits.h"
#include "control.h"
#include "acan.h"
#include "mxexp.h"
#include "mna_form.h"
#include "approx.h"
#include <assert.h>

/*
**	routines related to symbolic noise analysis.
**	In a addition to the spice file, we need 
**	a separated file contains the nosie sources.
*/

int
Circuit::read_nz_sources(char *file)
{
	FILE *fp;
	char key[128], n1[64], n2[64], val[64], df[16];
	char str[1024];

	assert(file);
	assert(theCkt);

	fp = fopen(file,"r");
    if (fp == NULL) 
	{
        sprintf(_buf,"Cann't find file: %s.",file);
        error_mesg(IO_ERROR,_buf);
        return -1;
    }

	while(fgets(str,1023,fp))
	{
		key[0] = 0; n1[0] = 0; n2[0] = 0; val[0] = 0; df[0] = 0;
		sscanf(str,"%s %s %s %s %s \n",key, n1, n2, val, df);
		printf("%s %s %s %s %s \n",key, n1, n2, val, df);
		theCkt->parse_noise(key,n1,n2,val,df);
	}

	fclose(fp);
	
	return true;
}

/*----------------------------------------------------
Spec: perform noise analysis
-----------------------------------------------------*/
void
ACAnalysisManager::noise_analysis()
{
    double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    double time2;
    double c_time;

    print_mesg("NOISE SPECTRAL DENSITY ANALYSIS ...");
    
	if(!matrix->index_mat)
        theCkt->build_mna();
    if(!matrix->sys_det)
        build_sys_ddd();
    
	time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"DDD construction time: " << c_time <<endl;
    
	if(!poly)
        build_expressions();
    
	time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    noise_freq_sweep();
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"Evaluation time time: " << c_time <<endl;
    //graph_display();
}

/*----------------------------------------------------
Spec: sweep over all the frequence range.
-----------------------------------------------------*/
void
ACAnalysisManager::noise_freq_sweep()
{
    double *freq = command->freq_pts;
    //bool visit_flag;

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

    if(!out_value)
        out_value = new complex[num_pts];
    if(!exact_value)
        exact_value = new complex[num_pts];

    for (int i = 0; i < num_pts; i++) 
	{
	
	    /* used for the DDD evaluation in tol level circuit */
		HasherTree *prec_ht    = new HasherTree(FALSE);
	    HasherTree *ht    = new HasherTree(FALSE);

	    /* used for the DDD evaluation in subcircuits */
	    theCkt->sub_init_ht();

		out_value[i] = exact_value[i] = 
			noise_evaluate
			(
				freq[i], 
				i, 
				prec_ht,
				ht
			);
	    delete ht;
    }
}


/*----------------------------------------------------
Spec: calculate noise speatral density
-----------------------------------------------------*/
complex
ExpList::noise_evaluate
( 
	double _freq, 
	int findex, 
	MNAMatrix *matrix, 
	HasherTree *ht,
	HasherTree *prec_ht
)
{
    Cofactor *cofact_aux = num_list;
    complex  num_value = 0.0, den_value = 0.0;
	complex  gain, result;

    if(!num_list || !den_list)
	{
        error_mesg(INT_ERROR,"Invalid num or den cofactor list");
        return complex(0.0, 0.0);
    }
    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
        num_value = num_value + 
			cofact_aux->evaluate( _freq, prec_ht, ht, FALSE );
	}
    num_val[findex] = num_value;
    //cout<<"num_val: "<<num_val[findex]<<endl;

    cofact_aux = den_list;
    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
        den_value = den_value + 
			cofact_aux->evaluate( _freq, prec_ht, ht, FALSE );
    
    den_val[findex] = den_value;
    //cout<<"den_val: "<<den_val[findex]<<endl;
    gain = num_value/den_value;

    /*
	  printf("gain (%d, %d) to output is %g\n",
	  input->node1, input->node2, norm(gain));
    */

    if(input->dep_freq == 0)
	{
		result = coeff * norm(gain);
		//cout<<"coeff: "<<coeff<<" result:"<< result<<endl;
    }
    else if(input->dep_freq == 1)
		result = (coeff/_freq) * norm(gain);
    else if(input->dep_freq == 2)
	{
		result = coeff * noise_source_func(_freq)* norm(gain);
    }
    else
		result = coeff * norm(gain);

    return result; 
}


complex 
ACAnalysisManager::noise_evaluate
( 
	double freq , 
	int freq_index, 
	HasherTree *ht,
	HasherTree *prec_ht
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
        cvalue += poly_list->noise_evaluate
			(
				freq,freq_index,
				matrix, 
				ht,
				prec_ht
			);
    }
    //cout<<"total cvalue: "<<cvalue<<endl;
    return cvalue;
}

/*
**	noise analysis using coefficient list 
*/

/*----------------------------------------------------
Spec: perform numerical noise analysis based on 
explicit coefficient expressions.
-----------------------------------------------------*/
void
ACAnalysisManager::coeff_noise_analysis()
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

    if(!theLabMap)
        coeff_build_expressions();
    
	time1 = 1.0 * clock() / CLOCKS_PER_SEC;
    coeff_noise_freq_sweep();
    time2 = 1.0 * clock() / CLOCKS_PER_SEC;
    c_time = time2 - time1;
    cout <<"Evaluation time time: " << c_time <<endl;
    //graph_display();
    print_mesg("END OF NUMERICAL AC ANALYSIS");
}

void
ACAnalysisManager::coeff_noise_freq_sweep()
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
        out_value = new complex[num_pts];

    for (int i = 0; i < num_pts; i++)
        out_value[i] = coeff_noise_evaluate(freq[i]);


    print_mesg("END OF FREQUENCY SWEEPING ");

}

complex
ACAnalysisManager::coeff_noise_evaluate( double _freq)
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
        cvalue += poly_list->coeff_noise_evaluate( _freq );
    }

    return cvalue;

}


/*----------------------------------------------------
Spec: calculate the numerical noise spectrum from 
	  coefficient lists;
-----------------------------------------------------*/
complex
ExpList::coeff_noise_evaluate( double _freq)
{
    Cofactor *cofact_aux = num_list;
    complex  num_value = 0.0, den_value = 0.0;
	complex  result;

    if(!num_list || !den_list)
	{
    	error_mesg(INT_ERROR, "Invalid num or den coefficient list");
		return complex(0.0, 0.0);
    }
    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
    	num_value = num_value + cofact_aux->coeff_evaluate( _freq);

    cofact_aux = den_list;
    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
    	den_value = den_value + cofact_aux->coeff_evaluate( _freq);

	complex gain = num_value/den_value;

	if(input->dep_freq == 0)
		result = coeff * norm(gain);
	else if(input->dep_freq == 1)
		result = (coeff/_freq) * norm(gain);
	else if(input->dep_freq == 2)
	{
		result = coeff * noise_source_func(_freq) * norm(gain);
		cout<<"noise source: "<<noise_source_func(_freq)<<endl;
		cout<<"result: "<<result<<endl;
	}
	else
		result = coeff * norm(gain);

    return result;

}

/*
**	In current SCAD^3, we use MapleV to do the symbolic 
**	algebraic manipulations to obtain the symbolic
**	noise models in terms of noise density functions
*/

/*----------------------------------------------------
Spec: output the symbolic transfer function to MapleV
-----------------------------------------------------*/
void
ACAnalysisManager::maple_cmd
( 
	ostream& out 
)
{

	int i;
    ExpList * poly_list = poly;

    if(!poly_list)
	{
        error_mesg(INT_ERROR,"Now symbolic polynomial expressions derived");
		return;
    }

    for(; poly_list ; poly_list = poly_list->next)
	{
        poly_list->maple_cmd( out );
    }


	out<<"num := sort(collect(";
	for(i=1; i <= num_exprs; i++)
	{
		if(i != num_exprs)
			out<<"poly_num"<<i<<"+ ";
		else
			out<<"poly_num"<<i;
	}
	out<<",f));"<<endl;

	out<<"den := sort(collect(dde_1 + ddo_1,f));"<<endl;
	out<<"readlib(C);"<<endl;
	out<<"h := num/den;"<<endl;
	out<<"C([out=h]);"<<endl;
	out<<"C([out=h],optimized);"<<endl;

}


/*----------------------------------------------------
Spec: output the symbolic transfer function to MapleV
-----------------------------------------------------*/
void
ExpList::maple_cmd(ostream& out)
{
    Cofactor *cofact_aux = num_list;
	char exp[64], exp2[64], res[64];

    if(!num_list || !den_list)
	{
    	error_mesg(INT_ERROR, "Invalid num or den coefficient list");
		return;
    }

    sprintf(exp,"eexp_%d",index);
    out << exp <<" := 0;"<<endl; 
    sprintf(exp,"oexp_%d",index);
    out << exp <<" := 0;"<<endl; 

    for(; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
    	cofact_aux->maple_cmd(out);
		sprintf(exp,"eexp_%d",index);
	
		if(cofact_aux->get_sign() > 0.0)
			out <<exp<<" := sort(collect("<<exp<<" + eexp,f));"<<endl;
		else
			out <<exp<<" := sort(collect("<<exp<<" - eexp,f));"<<endl;

		sprintf(exp,"oexp_%d",index);
		if(cofact_aux->get_sign() > 0.0)
			out <<exp<<" := sort(collect("<<exp<<" + oexp,f));"<<endl;
		else
			out <<exp<<" := sort(collect("<<exp<<" - oexp,f));"<<endl;

    }

    // for the numerator
    sprintf(exp,"eexp_%d",index);
    sprintf(exp2,"nne_%d",index);
    out <<exp2<<" := sort(collect("<<exp<<"^2,f));"<<endl;

    sprintf(exp,"oexp_%d",index);
    sprintf(exp2,"nno_%d",index);
    out <<exp2<<" := sort(collect("<<exp<<"^2,f));"<<endl;


    // for the denominator
    cofact_aux = den_list;

    sprintf(exp,"eexp_%d",index);
    out << exp <<" := 0;"<<endl; 
    sprintf(exp,"oexp_%d",index);
    out << exp <<" := 0;"<<endl; 

    for(; cofact_aux ; cofact_aux = cofact_aux->Next()){
    	cofact_aux->maple_cmd(out);
		sprintf(exp,"eexp_%d",index);
		if(cofact_aux->get_sign() > 0.0)
			out <<exp<<" := sort(collect("<<exp<<" + eexp,f));"<<endl;
		else
			out <<exp<<" := sort(collect("<<exp<<" - eexp,f));"<<endl;

		sprintf(exp,"oexp_%d",index);
		if(cofact_aux->get_sign() > 0.0)
			out <<exp<<" := sort(collect("<<exp<<" + oexp,f));"<<endl;
		else
			out <<exp<<" := sort(collect("<<exp<<" - oexp,f));"<<endl;
    }

    sprintf(exp,"eexp_%d",index);
    sprintf(exp2,"dde_%d",index);
    out <<exp2<<" := sort(collect("<<exp<<"^2,f));"<<endl;

    sprintf(exp,"oexp_%d",index);
    sprintf(exp2,"ddo_%d",index);
    out <<exp2<<" := sort(collect("<<exp<<"^2,f));"<<endl;

    // total result from this expression (only numerator)
    sprintf(exp,"nne_%d",index);
    sprintf(exp2,"nno_%d",index);
    sprintf(res,"poly_num%d",index);
    if(input->dep_freq)
		out <<res<<" := collect((("<<exp<<"+"<<exp2<<")/f)*"<<coeff<<",f);"
			<<endl;
	else
		out <<res<<" := collect(("<<exp<<"+"<<exp2<<")*"<<coeff<<",f);"<<endl;
}

/*----------------------------------------------------
Spec: output the symbolic transfer function to MapleV
-----------------------------------------------------*/
void
Cofactor::maple_cmd(ostream& out)
{
	if(!cddd || !coeff_list)
		return;

	coeff_list->maple_cmd(out);
}

void
CoeffList::maple_cmd( ostream& out )
{
	char str[256];
	double val;
		
	out<<"eexp := 0  ";
	for( int i = 0; i < max_order; i++)
	{
		if(coeff[i].coeff_value() != 0.0 && i%2 == 0)
		{
			val = real(coeff[i].coeff_value());
			if(val > 0.0)
				sprintf(str," + %g*f^%d ",val,i);
			else 
				sprintf(str,"%g*f^%d ", val,i);
			out<<str;
		}
	}
	out<<";"<<endl;

	out<<"oexp := 0  ";
	for( int j = 0; j < max_order; j++)
	{
		if(coeff[j].coeff_value() != 0.0 && j%2 == 1)
		{
			val = imag(coeff[j].coeff_value());
			if(val > 0.0)
				sprintf(str," + %g*f^%d ",val,j);
			else 
				sprintf(str,"%g*f^%d ", val,j);
			out<<str;
		}
	}
	out<<";"<<endl;
			
}
