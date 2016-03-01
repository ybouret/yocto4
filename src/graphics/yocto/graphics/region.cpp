#include "yocto/graphics/region.hpp"
#include <cstring>

namespace yocto
{
    namespace graphics
    {

        displacement:: ~displacement() throw()
        {
        }

        displacement:: displacement() throw() :
        r(),
        data()
        {
            memset(data,0,sizeof(data));
        }

        displacement:: displacement(const vertex &p) throw() :
        r(p),
        data()
        {
            memset(data,0,sizeof(data));
        }

        displacement:: displacement( const displacement &d ) throw() :
        r(d.r),
        data()
        {
            memcpy(data,d.data,sizeof(data));
        }

        displacement & displacement:: operator=(const displacement &d) throw()
        {
            r = d.r;
            memmove(data, d.data, sizeof(data) );
            return *this;
        }
    }
}