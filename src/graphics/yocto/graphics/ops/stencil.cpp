#include "yocto/graphics/ops/stencil.hpp"
#include <cstring>

namespace yocto
{
    namespace graphics
    {

        stencil::patch:: patch(const graphics::patch &p) throw() :
        graphics::patch(p),
        target(0),
        handle(0),
        f(gist::fill_with_zero),
        s()
        {
        }

        stencil::patch:: patch(const patch &p) throw() :
        graphics::patch(p),
        target(0),
        handle(p.handle),
        f(p.f),
        s()
        {
        }

        stencil::patch:: ~patch() throw()
        {
        }


        void stencil:: create(patches &sp, const graphics::patch &surface, threading::engine *server)
        {
            const size_t cpus = server ? server->size : 1;
            prepare_patches(sp,cpus,surface,true);
        }

        stencil::weights:: weights() throw() :
        value()
        {
            ldz();
        }

        stencil::weights:: ~weights() throw()
        {
        }

        void stencil::weights::ldz() throw()
        {
            memset(value, 0, sizeof(value) );
        }

        stencil::weights::weights(const weights &other) throw() :
        value()
        {
            memcpy(value, other.value, sizeof(value) );
        }


        stencil::weights & stencil::weights:: operator=(const stencil::weights &other ) throw()
        {
            memmove(value, other.value, sizeof(value) );
            return *this;
        }

    }
}
