#include "yocto/utest/run.hpp"
#include "yocto/math/stat/descr.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(stat)
{
    
    list<double> data1;
    for( size_t i=1; i <= 1000; ++i ) data1.push_back( alea<double>() );
    double ave1=0,sig1=0;
    double med1=0,abs1=0;
    math::compute_average_and_stddev(ave1,sig1,data1);
    math::compute_median_and_absdev(med1,abs1, data1);
    std::cerr << "ave=" << ave1 << ", sig=" << sig1 << std::endl;
    std::cerr << "med=" << med1 << ", abs=" << abs1 << std::endl;
}
YOCTO_UNIT_TEST_DONE()
