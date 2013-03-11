#include "yocto/utest/run.hpp"
#include "yocto/spade/visit-silo.hpp"
#include "yocto/spade/array2d.hpp"
#include "yocto/spade/rmesh.hpp"
#include "yocto/spade/region2d.hpp"

using namespace yocto;
using namespace spade;

typedef vertex2D<float>::type  V2f;
typedef vertex2D<double>::type V2d;

YOCTO_UNIT_TEST_IMPL(silo)
{
    Silo::File    db( "demo.silo", "Just A Demo");
    Silo::OptList optlist(2);
    
    optlist.AddTime(1);
    optlist.AddCycle(2);
    
    {
        array_db   adb;
        layout2D   L( coord2D(1,1), coord2D(4,5) );
        rmesh<layout2D,float> m2(L,adb);
        region2D<float>::type R( V2f(-1,-1), V2f(1,1) );
        m2.regular_map_to(R,m2);
        std::cerr << "mesh2d: " << L << std::endl;
        std::cerr << "X     : " << m2.X().as_layout() << std::endl;
        std::cerr << "Y     : " << m2.Y().as_layout() << std::endl;
        db.PutRectilinearMesh("mesh",m2,&optlist);
    }
    
    {
        array_db   adb;
        layout2D   L( coord2D(1,1), coord2D(4,5) );
        rmesh<layout2D,double> m2(L,adb);
        region2D<double>::type R( V2d(-1,-1), V2d(1,1) );
        m2.regular_map_to(R,m2);
        std::cerr << "mesh2d: " << L << std::endl;
        std::cerr << "X     : " << m2.X().as_layout() << std::endl;
        std::cerr << "Y     : " << m2.Y().as_layout() << std::endl;
        db.PutRectilinearMesh("meshd",m2,&optlist);
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
