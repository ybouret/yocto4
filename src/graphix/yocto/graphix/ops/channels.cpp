#include "yocto/graphix/ops/channels.hpp"

namespace yocto
{
    namespace graphix
    {
        channels::patch:: patch(const graphix::patch &p ) throw() :
        graphix::patch(p),
        source(0),
        target(0)
        {

        }

        channels::patch:: ~patch() throw()
        {
        }

        channels::patch::patch(const patch &other) throw() :
        graphix::patch(other),
        source(0),
        target(0)
        {

        }


        void channels:: create(patches              &chp,
                               const graphix::patch &surface,
                               threading::engine    *server)
        {
            const size_t cpus = server ? server->size : 1;
            prepare_patches(chp,cpus,surface,true);
        }
        
        
    }
}
