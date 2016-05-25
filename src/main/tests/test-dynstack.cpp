#include "yocto/stock/dynstack.hpp"
#include "yocto/utest/run.hpp"
#include "support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(dynstack)
{
    dynstack_of<double> ds0, ds1(100,as_capacity);
    std::cerr << "ds0.size=" << ds0.size() << std::endl;
    std::cerr << "ds1.size=" << ds1.size() << std::endl;

    ds0.push(10);
    ds1.push(10);

    for(size_t i=10+alea_leq(100);i>0;--i)
    {
        ds0.push( gen<double>::get() );
        ds1.push( gen<double>::get()  );
    }

    while(ds0.size())
    {
        ds0.pop();
        ds1.pop();
    }


    dynstack_of<string> dss;
    for(size_t i=10+alea_leq(100);i>0;--i)
    {
        dss.push( gen<string>::get() );
    }

    while(dss.size())
    {
        dss.pop();
    }

    for(size_t i=10+alea_leq(10);i>0;--i)
    {
        dss.push( gen<string>::get() );
    }
    std::cerr << "dss.size=" << dss.size() << "/items=" << dss.items() << std::endl;
    dss.start_frame();
    std::cerr << "dss.size=" << dss.size() << std::endl;

}
YOCTO_UNIT_TEST_DONE()
