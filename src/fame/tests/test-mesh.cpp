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
    }
}

YOCTO_UNIT_TEST_IMPL(mesh)
{
    arrays adb(8);
    adb.store( new array1D<double>("X",layout1D(-5,5)) );
    adb.store( new array1D<double>("Y",layout1D(-6,6)) );
    adb.store( new array1D<double>("Z",layout1D(-10,10)) );
    
    box<3,float> B(math::v3d<float>(1.0f,-2.0f,1.2f), math::v3d<float>(-1.0f,3.0f,-0.7f) );
    B.map<double>(adb);

    RectilinearMesh<1,double> rm1(adb); display_mesh(rm1);
    RectilinearMesh<2,double> rm2(adb); display_mesh(rm2);
    RectilinearMesh<3,double> rm3(adb); display_mesh(rm3);

}
YOCTO_UNIT_TEST_DONE()
