#ifndef YOCTO_GRAPHICS_OPS_PARTICLES_INCLUDED
#define YOCTO_GRAPHICS_OPS_PARTICLES_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"

namespace yocto
{
    namespace graphics
    {
        class active_vertex
        {
        public:
            ~active_vertex() throw();
            active_vertex(const vertex v);

            



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(active_vertex);
        };
    }
}

#endif
