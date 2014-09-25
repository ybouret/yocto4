#include "yocto/utest/run.hpp"
#include "yocto/fame/box.hpp"


using namespace yocto;
using namespace fame;

namespace  {
    
    
    template <typename AXIS>
    static inline void display( const AXIS &a )
    {
        std::cerr << '[';
        for(unit_t i=a.lower;i<=a.upper;++i)
        {
            std::cerr << ' ' <<  a[i];
        }
        std::cerr << ']' << std::endl;
    }
    
}

YOCTO_UNIT_TEST_IMPL(box)
{
    
    box<1,float> b1(1,-1);
    std::cerr << "b1=" << b1 << std::endl;
    array1D<double> x("x",layout1D(-5,5));
    b1.map(x);
    std::cerr << "x=" << x << std::endl;
    display(x);
    
    box<2,double> b2( math::v2d<double>(1,-2), math::v2d<double>(-1,3) );
    array1D<double> y("y",layout1D(-6,6));
    std::cerr << "b2=" << b2 << std::endl;
    b2.map(x,y);
    std::cerr << "y=" << y << std::endl;
    display(x);
    display(y);
    
    box<3,double> b3( math::v3d<double>(1,-2,1.2), math::v3d<double>(-1,3,-0.7) );
    array1D<double> z("z",layout1D(-10,10));
    std::cerr << "b3=" << b3 << std::endl;
    b3.map(x,y,z);
    std::cerr << "z=" << z << std::endl;
    display(x);
    display(y);
    display(z);

    arrays adb(8);
    adb.store( new array1D<double>("X",layout1D(-5,5)) );
    adb.store( new array1D<double>("Y",layout1D(-6,6)) );
    adb.store( new array1D<double>("Z",layout1D(-10,10)) );
    
    box<3,float> B(math::v3d<float>(1,-2,1.2), math::v3d<float>(-1,3,-0.7) );
    B.map<double>(adb);
    
}
YOCTO_UNIT_TEST_DONE()