#ifndef XDRAW_H
#define XDRAW_H

/*
 *    $RCSfile: xdraw.h,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/11/01 20:38:12 $
 */

#ifndef bool
#define bool boolean
#endif

#define BSIZE  512
#define XG_MAXVECTORS  64

#ifndef TRUE 
#define FALSE  0
#define TRUE  1
#endif

typedef enum {
    GRID_NONE = 0, 
    GRID_LIN = 1, 
    GRID_LOGLOG = 2, 
    GRID_XLOG = 3, 
    GRID_YLOG = 4
} GRIDTYPE;

typedef enum {
    PLOT_LIN = 0, 
    PLOT_COMB = 1, 
    PLOT_POINT = 2
} PLOTTYPE;

struct VECTOR {
    char  *v_Name;
    double  *v_Data;    /* Data. */
    int  v_Length;        /* Length of the vector. */
    struct VECTOR  *v_Next;    /* Link for list of plot vectors. */
    struct VECTOR  *v_Scale;/* If this has a non-standard scale */
};

class GraphOutput {

  protected:
    char   *FileName;
    char   *Title;
    int    num_pts; // number of points in the vector 
    char   *XLabel;
    char   *YLabel;
    GRIDTYPE  GridType;
    PLOTTYPE  PlotType;
    int      LineWidth;
    boolean Markers;

  public:

    struct VECTOR *Data;
    struct VECTOR *Scale;
    double XLims[2];
    double YLims[2];

    GraphOutput( int v_length );
    ~GraphOutput();
    char * & xTitle() { return Title;}
    char * & xFilename() { return FileName;}
    char * & xXlabel() { return XLabel;}
    char * & xYlabel() { return YLabel;}
    GRIDTYPE & xGridtype() { return GridType;}
    PLOTTYPE & xPlottype() { return PlotType;}
    int & xLinewidth() { return LineWidth;}
    boolean & xMarkers() { return Markers;}

    int xgraph_display(); 
    int xgraph_display(char  *_Title,
    char  *_XLabel,
    char  *_YLabel)
    {
        this->Title = _Title;
        this->XLabel = _XLabel;
        this->YLabel = _YLabel;
        return xgraph_display();
    }
};
#endif
