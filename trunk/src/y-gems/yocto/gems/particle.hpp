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
        class particle : public vslot
        {
        public:
            v3d<T> r; T w;
            v3d<T> v; T m;
            v3d<T> a;
            const uint_t id;     //!< for specific forces
            const uint_t type;   //!< for generic forces
            
            explicit particle() throw();
            virtual ~particle() throw();
            
        };
    }
    
}

#endif

