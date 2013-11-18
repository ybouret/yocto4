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

template <typename T>
T genv1()
{
    return T(10) * ( alea<T>() - T(0.5) );
}

template <typename T>
math::v2d<T> genv2()
{
    return math::v2d<T>( genv1<T>(), genv1<T>() );
}

template <typename T>
math::v3d<T> genv3()
{
    return math::v3d<T>( genv1<T>(), genv1<T>(), genv1<T>() );
}


#endif
