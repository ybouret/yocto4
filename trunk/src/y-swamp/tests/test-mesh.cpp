#include "yocto/utest/run.hpp"
#include "yocto/swamp/rectilinear-mesh.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/swamp/array3d.hpp"

using namespace yocto;
using namespace swamp;
template <typename LAYOUT>
static inline void display( const LAYOUT &L )
{
    std::cerr << "DIM= " << LAYOUT::DIMENSIONS << std::endl;
    std::cerr << L.lower << " -> " << L.upper << " = " << L.width << " | #=" << L.items << std::endl;
}

static inline coord1D rand1()
{
    return 20 - 40 * alea<double>();
}

static inline coord2D rand2()
{
    return coord2D( rand1(), rand1() );
}

static inline coord3D rand3() {    return coord3D( rand1(), rand1(), rand1() ); }


YOCTO_UNIT_TEST_IMPL(rmesh)
{
    array_db ADB;
    
    const layout2D L( rand2(), rand2() );
    rectilinear_mesh<float, layout2D> rmesh(ADB,L);
    
    display( rmesh );
    
    const array1D<float> &X = rmesh[0];
    display(X);
    const array1D<float> &Y = rmesh.Y();
    display(Y);
    try
    {
        display( rmesh.Z() );
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    
    
}
YOCTO_UNIT_TEST_DONE()

