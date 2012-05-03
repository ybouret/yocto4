#include "yocto/utest/run.hpp"
#include "yocto/swamp/rectilinear-mesh.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/swamp/array3d.hpp"

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


YOCTO_UNIT_TEST_IMPL(rmesh)
{
    array_db ADB;
    
    const layout2D L( coord2D(1,1), coord2D(5,10) );
    rectilinear_mesh<float, layout2D> rmesh(ADB,L);
    
    std::cerr << L << std::endl;
    
    const array1D<float> &X = rmesh[0];
    std::cerr << X.__layout() << std::endl;
    
    const array1D<float> &Y = rmesh.Y();
    std::cerr << Y.__layout() << std::endl;
    try
    {
        std::cerr << rmesh.Z().__layout() << std::endl;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
  
    typedef vertex2D<float>::type vtx;
    const region2D<float>::type   R( vtx(-2,-3), vtx(4,6) );
    
    rmesh.regular_map_to(R,L);
    
    std::cerr << "X=" << rmesh.X() << std::endl;
    std::cerr << "dX=" << rmesh.dX() << std::endl;
    std::cerr << "Y=" << rmesh.Y() << std::endl;
    std::cerr << "dY=" << rmesh.dY() << std::endl;

    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/swamp/curvilinear-mesh.hpp"

YOCTO_UNIT_TEST_IMPL(cmesh)
{
    array_db ADB;
    const layout3D L( coord3D(1,1,1), coord3D(5,10,15) );
    curvilinear_mesh<double, layout3D> cmesh(ADB,L);
    typedef vertex3D<double>::type vtx;
    const region3D<double>::type   R( vtx(-2,-3,-4), vtx(4,6,8) );

    cmesh.regular_map_to(R,L);
    
    
}
YOCTO_UNIT_TEST_DONE()

