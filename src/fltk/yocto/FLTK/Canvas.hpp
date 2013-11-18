#ifndef YOCTO_FLTK_CANVAS_INCLUDED
#define YOCTO_FLTK_CANVAS_INCLUDED 1

#include "yocto/FLTK/Axis.hpp"
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
            
            Axis   xaxis;
            Axis   yaxis;
            Axis   y2axis;
            Curves curves;
            Curves curves2;
            
            virtual void draw();
            
        private:
            static 
            void __draw( const Fl_Box &box, const Axis &X, const Axis &Y, const Curves &C );
            YOCTO_DISABLE_COPY_AND_ASSIGN(Canvas);
        };
        
    }
    
}

#endif
