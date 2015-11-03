#include "yocto/graphics/ops/grad.hpp"

namespace yocto
{

    namespace graphics
    {

        gradient:: patch:: ~patch() throw()
        {
        }

        gradient::patch:: patch(const graphics::patch &p) throw() : graphics::patch(p),
        gmax(0),
	target(0),
	source(0)
        {
        }


        gradient::patch:: patch(const patch &p) throw() :
        graphics::patch(p),
        gmax(p.gmax),
	target(p.target),
	source(p.source)
        {

        }


        void gradient::create(patches              &gp,
                              const graphics::patch &surface,
                              threading::engine    *server)
        {
            const size_t cpus = server ? server->size : 1;
            prepare_patches(gp,cpus,surface,true);
        }
        
        
        
    }
    
}
