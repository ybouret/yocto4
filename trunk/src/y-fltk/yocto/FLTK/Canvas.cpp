#include "yocto/FLTK/Canvas.hpp"

namespace yocto
{
    namespace FLTK
    {
        Canvas:: ~Canvas() throw()
        {
            
        }
        
        Canvas:: Canvas( int X, int Y, int W, int H, const char *l ) :
        Fl_Box(X,Y,W,H,l),
        xaxis(),
        yaxis(),
        y2axis(),
        curves(),
        curves2()
        {
        }
        
        
        void Canvas:: draw()
        {
            //------------------------------------------------------------------
            // draw the box
            //------------------------------------------------------------------
            Fl_Box::draw();
            
            //------------------------------------------------------------------
            // draw the curves
            //------------------------------------------------------------------
            __draw( *this, xaxis, yaxis,  curves);
            __draw( *this, xaxis, y2axis, curves2);
        }
        
        
        void Canvas:: __draw( const Fl_Box &box, const Axis &Xaxis, const Axis &Yaxis, const Curves &C )
        {
            //------------------------------------------------------------------
            // compute scaling factors
            //------------------------------------------------------------------
            const int W = box.w();
            const int H = box.h();
            const int X = box.x();
            const int Y = box.y();
            if( W <= 0 || H <= 0 ) return;
            const double xscale = (W-1)/Xaxis.length;
            const double yscale = (H-1)/Yaxis.length;
            const double ytop   = Y+H-1;
            
            fl_push_clip(X, Y, W, H);
            //------------------------------------------------------------------
            // iterate on curves
            //------------------------------------------------------------------
            for( Curves::const_iterator i = C.begin(); i != C.end(); ++i )
            {
                const Curve &crv = **i;
                if( crv.size() > 0 )
                {
                    fl_color( crv.color );
                    for( size_t j=2; j <= crv.size(); ++j )
                    {
                        const Point &p0 = crv[j-1];
                        const Point &p1 = crv[j];
                        
                        const double x0 = X+(p0.x - Xaxis.vmin) * xscale;
                        const double y0 = ytop-(p0.y - Yaxis.vmin) * yscale;
                        
                        const double x1 = X+(p1.x - Xaxis.vmin) * xscale;
                        const double y1 = ytop-(p1.y - Yaxis.vmin) * yscale;
                        fl_line( int(x0), int(y0), int(x1), int(y1) );
                    }
                }
            }
            fl_pop_clip();
        }
        
        
    }
}
