#ifndef YOCTO_FOVEA_DIMENSIONS_INCLUDED
#define YOCTO_FOVEA_DIMENSIONS_INCLUDED 1

#include "yocto/fovea/layout.hpp"
#include "yocto/math/v3d.hpp"

namespace yocto
{
    namespace fovea
    {
        typedef ptrdiff_t            coord1D;
        typedef layout_of<coord1D>   layout1D;
        template <typename T> struct vertex1D {
            typedef T type;
        };
        
        typedef math::v2d<ptrdiff_t>     coord2D;
        typedef layout_of<coord2D>       layout2D;
        template <typename T> struct     vertex2D {
            typedef math::v2d<T> type;
        };
        
        
        typedef math::v3d<ptrdiff_t>     coord3D;
        typedef layout_of<coord3D>       layout3D;
        template <typename T> struct     vertex3D {
            typedef math::v3d<T> type;
        };
    }
    
}

#endif

