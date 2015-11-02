#include "yocto/graphics/ops/stencil.hpp"
#include <cstring>

namespace yocto
{
    namespace graphics
    {
        void stencil::ldz() throw()
        {
            memset(weight, 0, sizeof(weight) );

        }

        stencil:: stencil() throw() :
        mode(fill_with_zero),
        weight()
        {
            ldz();
        }

        stencil:: ~stencil() throw() {}


        stencil:: stencil(const stencil &other) throw() :
        mode(other.mode),
        weight()
        {
            memcpy(weight,other.weight,sizeof(weight));
        }

        stencil & stencil:: operator=( const stencil &other ) throw()
        {
            mode=other.mode;
            memmove(weight,other.weight, sizeof(weight));
            return *this;
        }

    }
}
