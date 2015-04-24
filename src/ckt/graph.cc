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
 *    $RCSfile: graph.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/05/29 07:50:27 $
 */


#include <math.h>
#include "circuits.h"
#include "acan.h"

/*----------------------------------------------------
Spec: display the analysis result using xgraph program.
    the following procedure do the preparation for 
    the display.
-----------------------------------------------------*/
void
ACAnalysisManager::graph_display(char *_type)
{
    int i;
    if(!out_value || !num_pts)
        return; // no output
    
    OutFormat    format = outputs->format;

    if(_type){
        char *type = ToLower(_type);
        if(!strncmp(type,"mag",3))
            format = oMEG;
        else if(!strncmp(type,"pha",3))
            format = oPHASE;
        else if(!strncmp(type,"db",2))
            format = oDB;
        else if(!strncmp(type,"real",4))
            format = oREAL;
        else if(!strncmp(type,"imag",4))
            format = oIMAG;
        else if(!strncmp(type,"noise",5))
            format = oNOISE;
    }
        
    if(!graph_out)
        graph_out = new GraphOutput( num_pts );

    if(graph_out->Data->v_Next){
        delete [] graph_out->Data->v_Next->v_Data;
        delete graph_out->Data->v_Next;
        graph_out->Data->v_Next = NULL;
    }

    char str[64];
    if(outputs->type == dVOL){
        sprintf(str,"V");
        delete graph_out->xYlabel();
        graph_out->xYlabel() = CopyStr(str);
    }
    else{
        sprintf(str,"I");
        delete graph_out->xYlabel();
        graph_out->xYlabel() = CopyStr(str);
    }
        
    switch( format ){

    case oMEG:
        for( i = 0; i < num_pts; i++ ){
            graph_out->Data->v_Data[i] = magn(out_value[i]);
            graph_out->Scale->v_Data[i] = command->freq_pts[i];
        }
        strcat(str,"MEG");
        graph_out->xGridtype() = GRID_LOGLOG;
        break;

     case oREAL:
        for( i = 0; i < num_pts; i++ ){
            graph_out->Data->v_Data[i] = real(out_value[i]);
            graph_out->Scale->v_Data[i] = command->freq_pts[i];
        }
        strcat(str,"R");
        graph_out->xGridtype() = GRID_XLOG;
        break;

     case oIMAG:
        for( i = 0; i < num_pts; i++ ){
            graph_out->Data->v_Data[i] = imag(out_value[i]);
            graph_out->Scale->v_Data[i] = command->freq_pts[i];
        }
        strcat(str,"I");
        graph_out->xGridtype() = GRID_XLOG;
        break;

     case oPHASE:
        for( i = 0; i < num_pts; i++ ){
            graph_out->Data->v_Data[i] = 
                arg(out_value[i])*57.30;
            graph_out->Scale->v_Data[i] = command->freq_pts[i];
        }
        strcat(str,"P");
        graph_out->xGridtype() = GRID_XLOG;
        break;

     case oDB:
        for( i = 0; i < num_pts; i++ ){
            graph_out->Data->v_Data[i] = db20(out_value[i]);
            graph_out->Scale->v_Data[i] = command->freq_pts[i];
         }
        strcat(str,"DB");
        graph_out->xGridtype() = GRID_XLOG;
        break;

     case oNOISE:
        for( i = 0; i < num_pts; i++ ){
            graph_out->Data->v_Data[i] = real(out_value[i]);
            graph_out->Scale->v_Data[i] = command->freq_pts[i];
         }
        strcat(str,"(NOISE_SPETRAL_DENSITY)");
        graph_out->xGridtype() = GRID_XLOG;
        break;
        
     default:
            error_mesg(INT_ERROR,"Unknow output format!");
            break;
     }

     char str1[32];

     if(outputs->node1){
		 sprintf(str1,"(%d,%d)",outputs->node1,outputs->node2);
		 strcat(str,str1);
     }

     delete graph_out->xTitle();
     graph_out->xTitle() = CopyStr(str);

     graph_out->xgraph_display();
}
    
/*----------------------------------------------------
Spec: compare the result in out_value and the exact
      result in a graphic way.
-----------------------------------------------------*/
void
ACAnalysisManager::graph_compare(char *_type)
{
    int i;
    char str[128];
    double err, exa_val, simp_val;
    double db_err = 0.0, db_abs_err = 0.0;
	double phs_err = 0.0, phs_abs_err = 0.0, freq;
    if(!out_value || !num_pts)
        return; // no output
    
    OutFormat format = outputs->format;

    if(_type){
        char *type = ToLower(_type);
        if(!strncmp(type,"mag",3))
            format = oMEG;
        else if(!strncmp(type,"pha",3))
            format = oPHASE;
        else if(!strncmp(type,"db",2))
            format = oDB;
        else if(!strncmp(type,"real",4))
            format = oREAL;
        else if(!strncmp(type,"imag",4))
            format = oIMAG;
    }
        
    if(!graph_out)
        graph_out = new GraphOutput( num_pts );

    /* generate the second vectors */
    if(!graph_out->Data->v_Next){
        graph_out->Data->v_Next = new VECTOR;
        graph_out->Data->v_Next->v_Data = new double[num_pts];
        graph_out->Data->v_Next->v_Length = num_pts;
        graph_out->Data->v_Next->v_Next = NULL;
        graph_out->Data->v_Next->v_Scale = graph_out->Data->v_Scale;
        delete graph_out->Data->v_Name;
        sprintf(str,"Exact Result");
        graph_out->Data->v_Name = CopyStr(str); 
        sprintf(str,"Approximate Result");
        graph_out->Data->v_Next->v_Name = CopyStr(str);
    }

    if(outputs->type == dVOL){
        sprintf(str,"Voltage");
        delete graph_out->xYlabel();
        graph_out->xYlabel() = CopyStr(str);
    }
    else{
        sprintf(str,"Current");
        delete graph_out->xYlabel();
        graph_out->xYlabel() = CopyStr(str);
    }
        
   switch( format ){
   case oMEG:
        for( i = 0; i < num_pts; i++ ){
            graph_out->Data->v_Data[i] = 
            magn(exact_value[i]);
            graph_out->Data->v_Next->v_Data[i] = 
            magn(out_value[i]);
            graph_out->Scale->v_Data[i] = 
            command->freq_pts[i];
        }
        strcat(str,"MEG");
        graph_out->xGridtype() = GRID_LOGLOG;
        break;

    case oREAL:
        for( i = 0; i < num_pts; i++ ){
            graph_out->Data->v_Data[i] = 
            real(exact_value[i]);
            graph_out->Data->v_Next->v_Data[i] = 
            real(out_value[i]);
            graph_out->Scale->v_Data[i] = 
            command->freq_pts[i];
        }
        strcat(str,"R");
        graph_out->xGridtype() = GRID_XLOG;
        break;

	case oIMAG:
        for( i = 0; i < num_pts; i++ ){
            graph_out->Data->v_Data[i] = 
            imag(exact_value[i]);
            graph_out->Data->v_Next->v_Data[i] = 
            imag(out_value[i]);
            graph_out->Scale->v_Data[i] = 
            command->freq_pts[i];
        }
        strcat(str,"I");
        graph_out->xGridtype() = GRID_XLOG;
        break;

     case oPHASE:
        for( i = 0; i < num_pts; i++ ){
            exa_val = graph_out->Data->v_Data[i] = 
            arg(exact_value[i])*57.30;
            simp_val = graph_out->Data->v_Next->v_Data[i] =
            arg(out_value[i])*57.30;
            graph_out->Scale->v_Data[i] = 
            command->freq_pts[i];
            //err = fabs(exa_val - simp_val)/fabs(exa_val);
			err = fabs(exa_val - simp_val);
            if(err > phs_err)
			{
                phs_err = fabs(exa_val - simp_val)/fabs(exa_val);
				phs_abs_err = err;
				freq = command->freq_pts[i];
            }
        }
        strcat(str,"P");
        graph_out->xGridtype() = GRID_XLOG;
        sprintf(_buf,"relative error and abs err in Phase is %g and %g  at freq %g",
				phs_err, phs_abs_err, freq);
        print_mesg(_buf);
        break;

      case oDB:
        for( i = 0; i < num_pts; i++ ){
            exa_val = graph_out->Data->v_Data[i] = 
            db20(exact_value[i]);
            simp_val = graph_out->Data->v_Next->v_Data[i] =
            db20(out_value[i]);
            graph_out->Scale->v_Data[i] = 
            command->freq_pts[i];
            //err = fabs(exa_val - simp_val)/fabs(exa_val);
			err = fabs(exa_val - simp_val);
            if(err > db_err)
			{
                db_err = fabs(exa_val - simp_val)/fabs(exa_val);
				db_abs_err = err;
                freq = command->freq_pts[i];
            }
        }
        strcat(str,"DB");
        graph_out->xGridtype() = GRID_XLOG;
        sprintf(_buf,"relative error and abs err in DB is %g and %g at freq %g", 
				db_err, db_abs_err, freq);
        print_mesg(_buf);
        break;
            
     default:
			error_mesg(INT_ERROR,"Unknow output format!");
			break;
    }

    char str1[32];
    if(outputs->node1){
        sprintf(str1,"(%d,%d)",outputs->node1,outputs->node2);
        strcat(str,str1);
    }
    delete graph_out->xTitle();
    graph_out->xTitle() = CopyStr(str);

    graph_out->xgraph_display();
}
