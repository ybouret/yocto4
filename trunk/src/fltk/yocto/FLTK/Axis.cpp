#include "yocto/FLTK/Axis.hpp"

namespace yocto 
{
    namespace FLTK 
    {
        Axis:: Axis() throw() : vmin(0),vmax(1),length(vmax-vmin)
        {
            
        }
        
        Axis:: ~Axis() throw()
        {
        }
        
        void Axis:: set_min( double amin ) 
        {
            (double&)vmin   = amin;
            (double&)length = vmax - vmin;
        }
        
        void Axis:: set_max(double amax)
        {
            (double&)vmax   = amax;
            (double&)length = vmax - vmin;
        }
        
        void Axis:: set_range(double amin, double amax)
        {
            (double&)vmin   = amin;
            (double&)vmax   = amax;
            (double&)length = vmax - vmin;
        }
        
        void Axis:: autoscaleY( const Curve &C, const double extra )
        {
            if( C.size() > 0 )
            {
                double cmin = C[1].y;
                double cmax = cmin;
                for( size_t i=2; i <= C.size(); ++i )
                {
                    const double tmp = C[i].y;
                    if( tmp < cmin )
                        cmin = tmp;
                    if( tmp > cmax )
                        cmax = tmp;
                }
                const double amplitude = cmax - cmin;
                const double delta     = amplitude * extra;
                set_range( cmin - delta, cmax + delta );
            }
        }
    }
}
