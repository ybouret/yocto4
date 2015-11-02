
#include "yocto/graphics/ops/blur.hpp"

namespace yocto {
    
    namespace graphics
    {
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
        
        size_t blur::level = 1000;
        
    }
    
}
