#ifndef YOCTO_GFX_RAMP_INCLUDED
#define YOCTO_GFX_RAMP_INCLUDED 1

#include "yocto/gfx/color/data2rgba.hpp"

namespace yocto
{
    namespace gfx
    {

        class ramp : public data2rgba
        {
        public:
            float    vmin;
            float    vmax;
            virtual ~ramp() throw();

            RGBA compute(const float v) const throw();


        protected:
            explicit ramp(const RGBA *user_codes) throw();
            const RGBA *codes;
            virtual RGBA get(const void *addr) { return compute( *(const float *)addr ); }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ramp);
        };

    }
}


#endif
