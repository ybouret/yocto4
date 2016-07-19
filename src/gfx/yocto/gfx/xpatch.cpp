#include "yocto/gfx/xpatch.hpp"
#include "yocto/parallel/splitter.hpp"

namespace yocto
{
    namespace gfx
    {
        xpatch:: xpatch(const patch &p) :
        patch(p),
        vslot(),
        jid(0)
        {
        }

        xpatch:: xpatch(const xpatch &other) :
        patch(other),
        vslot(),
        jid(0)
        {
        }
        

        xpatch:: ~xpatch() throw()
        {
        }

    }

}

namespace yocto
{
    namespace gfx
    {
        xpatches:: xpatches(const patch &source, threading::dispatcher *srv) :
        _xpatches(),
        server(srv)
        {
            setup_from(source);
        }

        void xpatches:: setup_from(const patch &source)
        {
            parallel::build_patches(*this,server->levels(),source);
        }

        xpatches::xpatches(const patch &source) :
        _xpatches(),
        server( new threading::sequential_dispatcher() )
        {
            setup_from(source);
        }

        xpatches:: xpatches(const patch &source, const bool setVerbose) :
        _xpatches(),
        server( new threading::engine(setVerbose) )
        {
            setup_from(source);
        }

        xpatches:: ~xpatches() throw()
        {
        }
        
    }
}
