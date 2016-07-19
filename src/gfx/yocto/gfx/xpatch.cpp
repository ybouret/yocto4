#include "yocto/gfx/xpatch.hpp"
#include "yocto/parallel/splitter.hpp"

namespace yocto
{
    namespace gfx
    {
        xpatch:: xpatch(const patch &p) :
        patch(p),
        vslot(),
        jlk(),
        jid(0)
        {
        }

        xpatch:: xpatch(const xpatch &other) :
        patch(other),
        vslot(),
        jlk(),
        jid(0)
        {
        }
        

        xpatch:: ~xpatch() throw()
        {
        }

        void xpatch:: create(_xpatches         &xp,
                             const patch       &source,
                             threading::engine *server)
        {
            parallel::build_patches(xp,(server?server->size:1),source);
        }

    }
}
