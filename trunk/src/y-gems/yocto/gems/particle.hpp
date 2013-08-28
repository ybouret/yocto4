#ifndef YOCTO_GEMS_PARTICLE_INCLUDED
#define YOCTO_GEMS_PARTICLE_INCLUDED 1

#include "yocto/gems/v4d.hpp"

namespace yocto
{
    namespace gems
    {
     
        template <typename T>
        class particle
        {
        public:
            v4d<T> r; //!< position + 1/mass
            v4d<T> v; //!< velocity + mass
            v3d<T> a; //!< acceleration/force accumulator
            
            particle( T mass, bool is_mobile = true ) throw() :
            r(0,0,0,is_mobile ? 1/mass : 0 ),
            v(0,0,0,mass),
            a(0,0,0,0)
            {
            }
            
            ~particle() throw()
            {
            }
            
        };
    }
    
}

#endif

