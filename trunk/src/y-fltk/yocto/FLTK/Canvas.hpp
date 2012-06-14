#ifndef YOCTO_FLTK_CANVAS_INCLUDED
#define YOCTO_FLTK_CANVAS_INCLUDED 1

#include <FL/Fl_Box.H>
#include "yocto/string.hpp"

namespace yocto 
{
    namespace FLTK
    {
        
        class Canvas : public Fl_Box
        {
        public:
            explicit Canvas( int X, int Y, int W, int H, const char *l = 0 );
            virtual ~Canvas() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Canvas);
        };
        
    }
    
}

#endif
