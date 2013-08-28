#ifndef YOCTO_GEMS_PARTICLE_INCLUDED
#define YOCTO_GEMS_PARTICLE_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/math/v3d.hpp"

namespace yocto
{
    namespace gems
    {
     
        using namespace math;
        
        template <typename T>
        class particle
        {
        public:
            v3d<T> r; T w;
            v3d<T> v; T m;
            v3d<T> a;
            uint_t id;
            
             particle() throw();
            ~particle() throw();
            
        };
    }
    
}

#endif

