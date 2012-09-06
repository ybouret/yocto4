#include "yocto/utest/run.hpp"
#include "./main.hpp"

#include "yocto/spade/array1d.hpp"

YOCTO_UNIT_TEST_IMPL(array1D)
{
    const layout1D L1( RandCoord1D(), RandCoord1D());
    array1D<float> A1( L1 );

}
YOCTO_UNIT_TEST_DONE()
