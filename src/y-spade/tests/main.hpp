#ifndef YOCTO_TEST_SPADE_MAIN_INCLUDED
#define YOCTO_TEST_SPADE_MAIN_INCLUDED 1

#include "yocto/code/rand.hpp"
#include "yocto/spade/in1d.hpp"
#include "yocto/spade/in2d.hpp"
#include "yocto/spade/in3d.hpp"

using namespace yocto;
using namespace spade;

inline coord1D RandCoord1D()
{
    return ptrdiff_t(-20) + ptrdiff_t( alea_leq(40) );
}

inline coord2D RandCoord2D()
{
    return coord2D( RandCoord1D(), RandCoord1D() );
}

inline coord3D RandCoord3D()
{
    return coord3D( RandCoord1D(), RandCoord1D(), RandCoord1D() );
}

#endif
