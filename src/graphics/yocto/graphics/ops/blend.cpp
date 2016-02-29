#include "yocto/graphics/ops/blend.hpp"

namespace yocto
{
    namespace graphics
    {

        blend::patch::patch(const graphics::patch &p) throw() :
        graphics::patch(p),
        target(0),
        source(0)
        {
        }

        blend::patch:: ~patch() throw()
        {
        }

        blend::patch:: patch( const patch &p ) throw() :
        graphics::patch(p),
        target(0),
        source(0)
        {
        }


        void blend:: create(patches               &gp,
                            const graphics::patch &surface,
                            threading::engine    *server)
        {
            const size_t cpus = server ? server->size : 1;
            prepare_patches(gp,cpus,surface,true);
        }
    }
    
}
