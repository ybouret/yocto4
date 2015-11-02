#include "yocto/graphics/ops/grad.hpp"

namespace yocto
{

    namespace graphix
    {

        gradient:: patch:: ~patch() throw()
        {
        }

        gradient::patch:: patch(const graphix::patch &p) throw() : graphix::patch(p),
        gmax(0)
        {
        }


        gradient::patch:: patch(const patch &p) throw() :
        graphix::patch(p),
        gmax(p.gmax)
        {

        }


        void gradient::create(patches              &gp,
                              const graphix::patch &surface,
                              threading::engine    *server)
        {
            const size_t cpus = server ? server->size : 1;
            prepare_patches(gp,cpus,surface,true);
        }
        
        
        
    }
    
}
