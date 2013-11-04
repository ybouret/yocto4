#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/gfx/rgb.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class image : public singleton<image>
        {
        public:
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(image);
            virtual ~image() throw();
            explicit image();
            friend class singleton<image>;
            static const char name[];
			static const threading::longevity life_time = 100; // TODO: checl
        };
        
    }
}

#endif
