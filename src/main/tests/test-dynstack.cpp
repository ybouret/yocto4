#include "yocto/stock/dynstack.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(dynstack)
{
    dynstack_of<double> ds0, ds1(1,as_capacity);
    std::cerr << "ds0.size=" << ds0.size() << std::endl;
    std::cerr << "ds1.size=" << ds1.size() << std::endl;

    ds0.push(10);
    ds1.push(10);

    for(size_t i=10+alea_leq(100);i>0;--i)
    {
        ds0.push( alea_leq(100) );
        ds1.push( alea_leq(100) );
        
    }
}
YOCTO_UNIT_TEST_DONE()
