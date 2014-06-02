#ifndef YOCTO_PIXAR_INCLUDED
#define YOCTO_PIXAR_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"

namespace yocto
{
    namespace gfx
    {
        
        template <typename FORMAT>
        class pixar : public FORMAT, public bitmap
        {
        public:
            virtual ~pixar() throw() {}
            
        protected:
            explicit pixar( const FORMAT &fmt, unit_t width, unit_t height) throw() :
            FORMAT(fmt),
            bitmap(this->depth,width,height)
            {}
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(pixar);
        };
        
    }
}

#endif
