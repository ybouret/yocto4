#ifndef YOCTO_GRAPHIX_TYPES_INCLUDED
#define YOCTO_GRAPHIX_TYPES_INCLUDED 1

//#include "yocto/math/point2d.hpp"
#include "yocto/parallel/splitter.hpp"

namespace yocto
{
    namespace graphix
    {
        typedef point2d<unit_t>   vertex; //!< base class for operations
        typedef parallel::patch2D patch;  //!< base class for patches
    }
}

#endif
