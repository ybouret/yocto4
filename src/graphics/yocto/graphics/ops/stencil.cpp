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
        w(0),
        vmin(0),
        vmax(0),
        s()
        {
        }

        stencil::patch:: patch(const patch &p) throw() :
        graphics::patch(p),
        target(0),
        handle(p.handle),
        f(p.f),
        w(p.w),
        vmin(p.vmin),
        vmax(p.vmax),
        s()
        {
        }

        stencil::patch:: ~patch() throw()
        {
        }


        void stencil:: create(patches               &sp,
                              const graphics::patch &surface,
                              threading::engine     *server)
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
            memset(&value[0][0], 0, sizeof(value) );
        }

        stencil::weights::weights(const weights &other) throw() :
        value()
        {
            memcpy(&value[0][0], &other.value[0][0], sizeof(value) );
        }


        stencil::weights & stencil::weights:: operator=(const stencil::weights &other ) throw()
        {
            memmove(&value[0][0], &other.value[0][0], sizeof(value) );
            return *this;
        }

    }
}
