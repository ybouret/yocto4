#include "yocto/FLTK/Canvas.hpp"

namespace yocto 
{
    namespace FLTK
    {
        Canvas:: ~Canvas() throw()
        {
            
        }
        
        Canvas:: Canvas( int X, int Y, int W, int H, const char *l ) :
        Fl_Box(X,Y,W,H,l)
        {
        }
        
    }
}
