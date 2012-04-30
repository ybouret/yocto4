#ifndef YOCTO_SWAMP_IN1D_INCLUDED
#define YOCTO_SWAMP_IN1D_INCLUDED 1

#include "yocto/swamp/layout.hpp"
#include "yocto/swamp/region.hpp"

namespace yocto 
{
    namespace swamp
    {
        typedef unit_t               coord1D;
        typedef layout<coord1D>      layout1D;
        template <typename U> struct vertex1D { typedef U type; };
        template <typename U> struct region1D { typedef region<U,vertex1D> type; };
    }
}

#endif
