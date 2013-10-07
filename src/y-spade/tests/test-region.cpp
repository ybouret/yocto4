#include "yocto/utest/run.hpp"
#include "yocto/spade/region1d.hpp"
#include "yocto/spade/region2d.hpp"
#include "yocto/spade/region3d.hpp"

#include "./main.hpp"



YOCTO_UNIT_TEST_IMPL(region)
{
    region1D<float>::type  R1( genv1<float>(), genv1<float>() );
    std::cerr << "R1=" << R1 << std::endl;
    
    //typedef vertex2D<double>::type vtx2;
    region2D<double>::type         R2( genv2<double>(), genv2<double>() );
    std::cerr << "R2=" << R2 << std::endl;
    
    //typedef vertex2D<float>::type vtx3;
    region3D<float>::type         R3( genv3<float>(), genv3<float>() );
    std::cerr << "R3=" << R3 << std::endl;


}
YOCTO_UNIT_TEST_DONE()
