#ifndef YOCTO_GFX_RAMP_INCLUDED
#define YOCTO_GFX_RAMP_INCLUDED 1

#include "yocto/gfx/rgb.hpp"

namespace yocto
{
    namespace gfx
    {

        class ramp
        {
        public:
            explicit ramp( const rgb_t *user256rgb ) throw();
            virtual ~ramp() throw();

            //! cyclic get
            rgb_t operator[](size_t i) const throw();

            //! clamped to [0,1]
            rgb_t operator()(const double x) const throw();

        private:
            const rgb_t *colors;
            YOCTO_DISABLE_COPY_AND_ASSIGN(ramp);
        };
        
    }
}

#endif

