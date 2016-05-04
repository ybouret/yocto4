#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/fame/box.hpp"

#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace fame;

namespace {
    template <typename MESH>
    static inline void display_mesh( const MESH &mesh )
    {
        std::cerr << "Mesh DIM="   << MESH::DIMENSIONS << std::endl;
        std::cerr << "#vertices="  << mesh.vertices.size << std::endl;
        for(size_t i=0;i<mesh.dimensions;++i)
        {
            std::cerr << "\tlabel"  << i << " = " << mesh.get_label(i) << std::endl;
            std::cerr << "\tunits"  << i << " = " << mesh.get_units(i) << std::endl;

        }
    }
}

YOCTO_UNIT_TEST_IMPL(mesh)
{
    arrays adb(8);
    adb.store( new array1D<double>("X",layout1D(-5,5)) );
    adb.store( new array1D<double>("Y",layout1D(-6,6)) );
    adb.store( new array1D<double>("Z",layout1D(-10,10)) );
    
    box<3,float> B(point3d<float>(1.0f,-2.0f,1.2f), point3d<float>(-1.0f,3.0f,-0.7f) );
    B.map_rect<double>(adb);

    //RectilinearMesh<layout1D,double> rm1(adb,"mesh1d"); display_mesh(rm1);
    //OldRectilinearMesh<2,double> rm2(adb,"mesh2d"); display_mesh(rm2);
    //OldRectilinearMesh<3,double> rm3(adb,"mesh3d"); display_mesh(rm3);

}
YOCTO_UNIT_TEST_DONE()
