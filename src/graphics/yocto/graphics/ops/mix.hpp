#ifndef YOCTO_OPS_MIX_INCLUDED
#define YOCTO_OPS_MIX_INCLUDED 1

#include "yocto/graphics/rgb.hpp"

namespace yocto
{
    namespace graphics
    {

        struct mix
        {
            //! approx (X*(255-alpha)+Y*(alpha))/255
            static inline
            uint8_t blend(const uint8_t X, const uint8_t Y, const uint8_t alpha) throw()
            {
                return (unsigned(X)*unsigned(255-alpha)+unsigned(Y)*unsigned(alpha))/255;
            }

            static inline
            RGB blend(const RGB &X, const RGB &Y, const uint8_t alpha) throw()
            {
                const unsigned WY(alpha);
                const unsigned WX(255-alpha);
                return RGB(
                           (unsigned(X.r)*WX + unsigned(Y.r)*WY)/255,
                           (unsigned(X.g)*WX + unsigned(Y.g)*WY)/255,
                           (unsigned(X.b)*WX + unsigned(Y.b)*WY)/255
                );
            }

        };

    }
}

#endif
