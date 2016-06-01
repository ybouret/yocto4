#ifndef YOCTO_GRAPHICS_OPS_BLUR_INCLUDED
#define YOCTO_GRAPHICS_OPS_BLUR_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"

namespace yocto
{
    namespace graphics
    {

        class blur
        {
        public:
            explicit blur( float sig);
            virtual ~blur() throw();

            const unit_t  w;
        private:
            const  float *weight;
            vector<float> __data;
            YOCTO_DISABLE_COPY_AND_ASSIGN(blur);
        };

    }
}

#endif
