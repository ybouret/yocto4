#ifndef YOCTO_FLTK_CANVAS_INCLUDED
#define YOCTO_FLTK_CANVAS_INCLUDED 1

#include "yocto/FLTK/Curve.hpp"

#include <FL/Fl_Box.H>

namespace yocto 
{
    namespace FLTK
    {
        
        class Canvas : public Fl_Box
        {
        public:
            explicit Canvas( int X, int Y, int W, int H, const char *l = 0 );
            virtual ~Canvas() throw();
            
            Curves curves;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Canvas);
        };
        
    }
    
}

#endif
