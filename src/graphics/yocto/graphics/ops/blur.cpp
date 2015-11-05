
#include "yocto/graphics/ops/blur.hpp"
#include <cfloat>

namespace yocto {
    
    namespace graphics
    {
#if 0
        blur::patch::patch(const graphics::patch &p) throw() :
        graphics::patch(p),
        handle(0),
        blurred(0),
        sigma(1)
        {
        }
        
        blur::patch:: ~patch() throw()
        {
        }
        
        blur::patch::patch(const patch &p) throw() :
        graphics::patch(p),
        handle(p.handle),
        blurred(p.blurred),
        sigma(p.sigma)
        {
            
        }
        
        void blur::create(patches &blr, const graphics::patch &surface, threading::engine *server)
        {
            const size_t cpus = server ? server->size : 1;
            prepare_patches(blr,cpus,surface,true);
        }
        
        const blur::real_type blur::amplitude = std::sqrt( -2.0f*std::log(FLT_EPSILON) );
#endif
        
    }
    
}
