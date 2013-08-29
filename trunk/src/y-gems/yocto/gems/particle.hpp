#ifndef YOCTO_GEMS_PARTICLE_INCLUDED
#define YOCTO_GEMS_PARTICLE_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/container/vslot.hpp"
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
            v3d<T> r; const T w;
            v3d<T> v; const T m;
            v3d<T> a;
            const uint_t uuid;   //!< for specific forces and data
            const uint_t type;   //!< for generic forces
            
            particle() throw();
            ~particle() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(particle);
        };
    }
    
}

#endif

