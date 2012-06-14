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
    }
}
