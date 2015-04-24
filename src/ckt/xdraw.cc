
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
 *    $RCSfile: xdraw.cc,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/05/20 04:22:01 $
 */


double XLims[2], YLims[2];
struct VECTOR    *Data, *Scale;

#include <math.h>
#include "globals.h"
#include "xdraw.h"

/*----------------------------------------------------
Spec: the constructor and destructor
-----------------------------------------------------*/
GraphOutput::GraphOutput( int v_length )
{
    

    // the default values used for xgraph display
    XLims[0] = 1e35;
    XLims[1] = 1;
    YLims[0] = 1e35;
    YLims[1] = -1e35;
    FileName = CopyStr("_xgraph.in");
    Title = CopyStr("SCAD3 GRAPH OUTPUT");
    XLabel = CopyStr("Frequency Hz");
    YLabel = CopyStr("Response");
    GridType = GRID_LIN;
    PlotType = PLOT_LIN;
    LineWidth = 1;
    Markers = TRUE;

    Data = new VECTOR;
    Scale = new VECTOR;

    Data->v_Name = CopyStr("Response");
    Scale->v_Name = CopyStr("Frequency Hz");

	Data->v_Length = v_length;
	Scale->v_Length = v_length;

    Data->v_Data = new double[v_length];
    Scale->v_Data = new double[v_length];

	Data->v_Data[0] = 0.0;
	Scale->v_Data[0] = 0.0;
	Data->v_Next = NULL;
	Scale->v_Next = NULL;
	Data->v_Scale = Scale;
	Scale->v_Scale = NULL;
}

GraphOutput::~GraphOutput()
{
    delete [] FileName;
    delete [] XLabel;
    delete [] YLabel;
    delete [] Title;

    delete [] Data->v_Data;
    delete [] Scale->v_Data;
    delete [] Data->v_Name;
    delete [] Scale->v_Name;

    delete Data;
    delete Scale;

}


/*----------------------------------------------------
    FuncName: xgraph_display
    Spec: translate the requirement output requirement 
    into the xgraph input format and invoke xgraph
    to display the result.
 -----------------------------------------------------
    Inputs: 
    Outputs:

    Global Variblas used:
----------------------------------------------------*/
int
GraphOutput::xgraph_display()
{
    char  Buf[BSIZE];
    FILE  *File;
    bool  NoGrid;
    bool  XLog, YLog;
    int  i;
    struct VECTOR  *v, *scale;
    double  xval, yval;
    int  NumVecs = 0;

    for(v = Data; v; v = v->v_Next)
        NumVecs++;

    if(NumVecs == 0) 
	{
        error_mesg(INT_ERROR,"No data vector to plot.");
        return -1;
    }
    else if(NumVecs > XG_MAXVECTORS) 
	{
        error_mesg(INT_ERROR,"too many vectors for Xgraph.");
        return -1;
    }

    switch(GridType) 
	{

    case GRID_LIN:
        NoGrid = XLog = YLog = FALSE;

        break;

    case GRID_XLOG:
        XLog = TRUE;
        NoGrid = YLog = FALSE;

        break;

    case GRID_YLOG:
        YLog = TRUE;
        NoGrid = XLog = FALSE;

        break;

    case GRID_LOGLOG:
        XLog = YLog = TRUE;
        NoGrid = FALSE;

        break;

    case GRID_NONE:
        NoGrid = TRUE;
        XLog = YLog = FALSE;

        break;

    default:
        error_mesg(INT_ERROR, "Grid type unsupported by Xgraph.");
        return -1;    /* error code */
    }

    File = fopen(FileName, "w");
    if(File == NULL) 
	{
        sprintf(_buf, "Error: Can't open file %s to write.\n", FileName);
        error_mesg(IO_ERROR,_buf);
        return -1;    /* error code */
    }

    if(Title)
        fprintf(File, "TitleText: %s\n", Title);
    if(XLabel)
        fprintf(File, "XUnitText: %s\n", XLabel);
    if(YLabel)
        fprintf(File, "YUnitText: %s\n", YLabel);

    if(NoGrid) 
	{
        fprintf(File, "Ticks: True\n");
    }

    if(XLog)
        fprintf(File, "LogX: True\n" );
    
	if(XLims)
	{
        fprintf(File, "XLowLimit:  %e\n", XLims[0]);
        fprintf(File, "XHighLimit: %e\n", XLims[1]);
    }

    if(YLog)
        fprintf(File, "LogY: True\n");
    
	if(YLims) 
	{
        fprintf(File, "YLowLimit:  %e\n", YLims[0]);
        fprintf(File, "YHighLimit: %e\n", YLims[1]);
    }

    fprintf(File, "LineWidth: %d\n", LineWidth);
    fprintf(File, "BoundBox: True\n");

    if(PlotType == PLOT_COMB) 
	{
        fprintf(File, "BarGraph: True\n");
        fprintf(File, "NoLines: True\n");
    }
    else if(PlotType == PLOT_POINT) 
	{
        if( Markers) 
		{
            fprintf(File, "Markers: True\n");
        }
        else 
		{
            fprintf(File, "LargePixels: True\n");
        }

        fprintf(File, "NoLines: True\n");
    }

    for(v = Data; v; v = v->v_Next) 
	{
        scale = v->v_Scale;

        if (v->v_Name) 
		{
            fprintf(File, "\"%s\"\n", v->v_Name);
        }

        for (i = 0; i < scale->v_Length; i++) 
		{
            xval = scale->v_Data[i];
            yval = v->v_Data[i];

            fprintf(File, "% e % e\n", xval, yval);
        }
		
        fprintf( File, "\n" );
    }

    (void) fclose(File);

    (void) sprintf(Buf, "xgraph %s &", FileName );
    (void) system(Buf);

    return 0;
}
