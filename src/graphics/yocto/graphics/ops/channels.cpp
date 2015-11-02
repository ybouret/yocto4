#include "yocto/graphics/ops/channels.hpp"

namespace yocto
{
    namespace graphics
    {
        channels::patch:: patch(const graphics::patch &p ) throw() :
        graphics::patch(p),
        source(0),
        target(0)
        {

        }

        channels::patch:: ~patch() throw()
        {
        }

        channels::patch::patch(const patch &other) throw() :
        graphics::patch(other),
        source(0),
        target(0)
        {

        }


        void channels:: create(patches               &chp,
                               const graphics::patch &surface,
                               threading::engine    *server)
        {
            const size_t cpus = server ? server->size : 1;
            prepare_patches(chp,cpus,surface,true);
        }
        
        
    }
}
