#include "yocto/graphics/ops/stencil.hpp"
#include <cstring>

namespace yocto
{
    namespace graphics
    {
        stencil:: stencil() throw() : v()
        {
            memset(&v[0][0],0,sizeof(v));
        }

        stencil:: ~stencil() throw()
        {
        }


        stencil:: stencil(const stencil &other) throw() : v()
        {
            memcpy( &v[0][0], & other.v[0][0], sizeof(v));
        }

        stencil & stencil:: operator=(const stencil &other) throw()
        {
            memmove( &v[0][0], & other.v[0][0], sizeof(v));
            return *this;
        }

        std::ostream & operator<<( std::ostream &os, const stencil &S )
        {
            os << '[';
            os << S.v[0][0] << ' ' << S.v[0][1] << ' ' << S.v[0][2] << ';';
            os << S.v[1][0] << ' ' << S.v[1][1] << ' ' << S.v[1][2] << ';';
            os << S.v[2][0] << ' ' << S.v[2][1] << ' ' << S.v[2][2] << ']';
            return os;
        }


    }
}