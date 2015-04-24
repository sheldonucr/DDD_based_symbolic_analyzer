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
 *    $RCSfile: dddgc.cc,v $
 *    $Revision: 1.7 $
 *    $Date: 2002/11/21 02:40:15 $
 */

#include "circuits.h"
#include "acan.h"
#include "mxexp.h"
#include "mna_form.h"

/* 
    Perform the garbage collection during the fcoefficient DDD
    node elimination.
*/


void
ACAnalysisManager::forceGC(char *type)
{ 
	
	if(!type)
	{
		return;
	}
	if(!strcmp(type,"fcoeff"))
	{
		fcoeff_forceGC();
	}
	if(!strcmp(type,"sfcoeff"))
	{
		sfcoeff_forceGC();
	}
	if(!strcmp(type,"cddd"))
	{
		cddd_forceGC();
	}
	if(!strcmp(type,"sddd"))
	{
		sddd_forceGC();
	}
			
}

void
ACAnalysisManager::forceGCForAll()
{

	Cofactor *cofact_aux = NULL;
	
	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
		cofact_aux->keep_cddd();
		cofact_aux->keep_sddd();
		cofact_aux->keep_fcoeff_ddd();
		cofact_aux->keep_sfcoeff_ddd();
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
		cofact_aux->keep_cddd();
		cofact_aux->keep_sddd();
		cofact_aux->keep_fcoeff_ddd();
		cofact_aux->keep_sfcoeff_ddd();
	}
		
    print_mesg("\nExplicitly force GC for all ddds ...");
	ddd_mgr->GC();

	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
		cofact_aux->unkeep_cddd();
		cofact_aux->unkeep_sddd();
		cofact_aux->unkeep_fcoeff_ddd();
		cofact_aux->unkeep_sfcoeff_ddd();
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{
		cofact_aux->unkeep_cddd();
		cofact_aux->unkeep_sddd();
		cofact_aux->unkeep_fcoeff_ddd();
		cofact_aux->unkeep_sfcoeff_ddd();
	}
	
}


void 
ACAnalysisManager::coeff_GC()
{
#ifdef _DEBUG
    sprintf(_buf,"DDD node: %d",ddd_mgr->NumNodes());
    print_mesg(_buf);
#endif
    if(ddd_mgr->NumNodes() > Max_ddd_size)
	{
		Cofactor *cofact_aux = NULL;
	
		for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
		{			
			cofact_aux->keep_coeff_ddd();			
		}

		for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
		{			
			cofact_aux->keep_coeff_ddd();			
		}
                
		print_raw("\nImplicitly force GC for coeff DDDs...");
		ddd_mgr->GC();

		for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
		{			
			cofact_aux->unkeep_coeff_ddd();			
		}

		for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
		{			
			cofact_aux->unkeep_coeff_ddd();			
		}
                        
		print_mesg("done.");
    }
}

void 
ACAnalysisManager::fcoeff_GC()
{
#ifdef _DEBUG
    sprintf(_buf,"DDD node: %d",ddd_mgr->NumNodes());
    print_mesg(_buf);
#endif
    if(ddd_mgr->NumNodes() > Max_ddd_size)
	{
		Cofactor *cofact_aux = NULL;

		for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
		{			
			cofact_aux->keep_fcoeff_ddd();			
		}

		for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
		{			
			cofact_aux->keep_fcoeff_ddd();			
		}
                       
		print_raw("\nImplicitly force GC for fcoeff DDDs...");
		ddd_mgr->GC();
		for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
		{			
			cofact_aux->unkeep_fcoeff_ddd();			
		}

		for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
		{			
			cofact_aux->unkeep_fcoeff_ddd();			
		}
       
		print_mesg("done.");
    }
}


void 
ACAnalysisManager::fcoeff_forceGC()
{
	Cofactor *cofact_aux = NULL;

	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->keep_fcoeff_ddd();			
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->keep_fcoeff_ddd();			
	}

    
    print_mesg("\nExplicitly force GC for fcoeff DDDs ...");
    ddd_mgr->GC();

   	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->unkeep_fcoeff_ddd();			
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->unkeep_fcoeff_ddd();			
	}
    print_mesg("done.");
}

void 
ACAnalysisManager::coeff_forceGC()
{

   	Cofactor *cofact_aux = NULL;

	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->keep_coeff_ddd();			
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->keep_coeff_ddd();			
	}

    print_mesg("\nExplicitly force GC for coeff DDDs ...");
    ddd_mgr->GC();
	

	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->unkeep_coeff_ddd();			
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->unkeep_coeff_ddd();			
	}
}

void
ACAnalysisManager::sfcoeff_forceGC()
{
	Cofactor *cofact_aux = NULL;

	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->keep_sfcoeff_ddd();			
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->keep_sfcoeff_ddd();			
	}

    print_mesg("\nExplicitly force GC for simplified fcoeff DDDs ...");
    ddd_mgr->GC();

   	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->unkeep_sfcoeff_ddd();			
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->unkeep_sfcoeff_ddd();			
	}

    print_mesg("done.");
}


void 
ACAnalysisManager::cddd_forceGC()
{
   	Cofactor *cofact_aux = NULL;

	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->keep_cddd();			
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->keep_cddd();			
	}

    print_mesg("\nExplicitly force GC for complex DDDs ...");
    ddd_mgr->GC();
   
	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->unkeep_cddd();			
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->unkeep_cddd();			
	}

    print_mesg("done.");
}

void 
ACAnalysisManager::sddd_forceGC()
{
	Cofactor *cofact_aux = NULL;

	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->keep_sddd();			
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->keep_sddd();			
	}

    print_mesg("\nExplicitly force GC for simplified complex DDDs ...");
    ddd_mgr->GC();
	for(cofact_aux = poly->num_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->unkeep_sddd();			
	}

	for(cofact_aux = poly->den_list; cofact_aux ; cofact_aux = cofact_aux->Next())
	{			
		cofact_aux->unkeep_sddd();			
	}

    print_mesg("done.");
}
