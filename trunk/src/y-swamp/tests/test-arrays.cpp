#include "yocto/utest/run.hpp"

#include "yocto/swamp/array3d.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/swamp/factory.hpp"

using namespace yocto;
using namespace swamp;

static inline coord1D rand1()
{
    return 20 - 40 * alea<double>();
}

static inline coord2D rand2()
{
    return coord2D( rand1(), rand1() );
}

static inline coord3D rand3() {    return coord3D( rand1(), rand1(), rand1() ); }

YOCTO_UNIT_TEST_IMPL(arrays)
{
    
    factory<layout1D> F1;
    F1.record( typeid(array1D<float>), array1D<float>::ctor, array1D<float>::dtor );
    F1.use< array1D<double> >();
    array_db ADB;
    {
        const layout1D L( rand1(), rand1() );
        F1.produce("flt1", L, typeid(array1D<float>), ADB);
        F1.produce("dbl1", L, typeid(array1D<double>), ADB);
    }
    
}
YOCTO_UNIT_TEST_DONE()
