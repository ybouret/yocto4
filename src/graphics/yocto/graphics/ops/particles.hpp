#ifndef YOCTO_GRAPHICS_OPS_PARTICLES_INCLUDED
#define YOCTO_GRAPHICS_OPS_PARTICLES_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace graphics
    {
        class active_vertex : public counted_object
        {
        public:
            typedef arc_ptr<active_vertex> ptr;

            ~active_vertex() throw();
            active_vertex(const vertex v);


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(active_vertex);
        };
    }
}

#endif
