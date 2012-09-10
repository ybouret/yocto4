#include "yocto/utest/run.hpp"
#include "yocto/spade/rmesh.hpp"

#include "./main.hpp"

YOCTO_UNIT_TEST_IMPL(mesh)
{
    
    {
        array_db   adb;
        layout1D   L( RandCoord1D(), RandCoord1D());
        rmesh<layout1D,double> m1(L,adb);
        std::cerr << "mesh1d: " << L << std::endl;
        std::cerr << "X     : " << m1.X().as_layout() << std::endl;
    }
    
    {
        array_db   adb;
        layout2D   L( RandCoord2D(), RandCoord2D());
        rmesh<layout2D,float> m2(L,adb);
        std::cerr << "mesh2d: " << L << std::endl;
        std::cerr << "X     : " << m2.X().as_layout() << std::endl;
        std::cerr << "Y     : " << m2.Y().as_layout() << std::endl;
    }
    
    
    {
        array_db   adb;
        layout3D   L( RandCoord3D(), RandCoord3D());
        rmesh<layout3D,ptrdiff_t> m3(L,adb);
        std::cerr << "mesh3d: " << L<< std::endl;
        std::cerr << "X     : " << m3.X().as_layout() << std::endl;
        std::cerr << "Y     : " << m3.Y().as_layout() << std::endl;
        std::cerr << "Z     : " << m3.Z().as_layout() << std::endl;

    }

}
YOCTO_UNIT_TEST_DONE()

