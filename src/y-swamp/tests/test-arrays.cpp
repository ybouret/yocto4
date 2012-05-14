#include "yocto/utest/run.hpp"

#include "yocto/swamp/array3d.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/swamp/factory.hpp"

using namespace yocto;
using namespace swamp;

static inline coord1D rand1()
{
    return coord1D(20 - 40 * alea<double>());
}

static inline coord2D rand2()
{
    return coord2D( rand1(), rand1() );
}

static inline coord3D rand3() {    return coord3D( rand1(), rand1(), rand1() ); }

YOCTO_UNIT_TEST_IMPL(arrays)
{
    
    array_db ADB;

   
    factory<layout1D> F1;
    F1.use< array1D<float>  >();
    F1.use< array1D<double> >();
    F1.use< array1D<unit_t> >();

    {
        const layout1D L( rand1(), rand1() );
        F1.produce("flt1", L, typeid(array1D<float>),  ADB);
        F1.produce("dbl1", L, typeid(array1D<double>), ADB);
        F1.make< array1D<unit_t> > ("int1", L, ADB);
        array1D<float> &A = ADB["flt1"].as< array1D<float> >();
        (void)A;
    }
    
    
    factory<layout2D> F2;
    F2.use< array2D<float> >();
    F2.use< array2D< geom::v2d<double> > >();
    {
        const layout2D L( rand2(), rand2() );
        F2.make< array2D<float> >( "flt2", L, ADB );
        F2.make< array2D< geom::v2d<double> > > ( "vec2", L, ADB );
    }
    
    factory<layout3D> F3;
    F3.use< array3D<float> >();
    {
        const layout3D L( rand3(), rand3() );
        F3.make< array3D<float> >( "dbl3", L, ADB );
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
