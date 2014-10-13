#include "yocto/utest/run.hpp"
#include "yocto/fame/vertices.hpp"
#include "yocto/fame/box.hpp"


using namespace yocto;
using namespace fame;
using namespace math;


YOCTO_UNIT_TEST_IMPL(vtx)
{
    
    arrays adb(8);
    adb.store( new array1D<double>("X",layout1D(-5,5)) );
    adb.store( new array1D<double>("Y",layout1D(-6,6)) );
    adb.store( new array1D<double>("Z",layout1D(-10,10)) );
    
    box<3,float> B(math::v3d<float>(1,-2,1.2), math::v3d<float>(-1,3,-0.7) );
    B.map<double>(adb);
    
    Vertex<1,double>::List v1;
    
    v1.Rectilinear( adb["X"].as< array1D<double> >());
    
    Vertex<2,double>::List v2;
    v2.Rectilinear( adb["X"].as< array1D<double> >(), adb["Y"].as< array1D<double> >());
    
    Vertex<3,double>::List v3;
    v3.Rectilinear(adb["X"].as< array1D<double> >(),
                   adb["Y"].as< array1D<double> >(),
                   adb["Z"].as< array1D<double> >());
    
    
}
YOCTO_UNIT_TEST_DONE()
