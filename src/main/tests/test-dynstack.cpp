#include "yocto/stock/dynstack.hpp"
#include "yocto/utest/run.hpp"
#include "support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(dynstack)
{
    dynstack_of<double> ds0, ds1(100,as_capacity);
    std::cerr << "ds0.height=" << ds0.height() << std::endl;
    std::cerr << "ds1.height=" << ds1.height() << std::endl;

    ds0.push(10);
    ds1.push(10);

    for(size_t i=10+alea_leq(100);i>0;--i)
    {
        ds0.push( gen<double>::get() );
        ds1.push( gen<double>::get()  );
    }

    while(ds0.height())
    {
        ds0.pop();
        ds1.pop();
    }


    dynstack_of<string> dss;
    for(size_t i=10+alea_leq(100);i>0;--i)
    {
        dss.push( gen<string>::get() );
    }

    while(dss.height())
    {
        dss.pop();
    }

    for(size_t i=10+alea_leq(10);i>0;--i)
    {
        dss.push( gen<string>::get() );
    }
    std::cerr << "dss.height=" << dss.height() << "/size=" << dss.size() << std::endl;
    dss.start_frame();
    std::cerr << "dss.height=" << dss.height() << "/size=" << dss.size() << std::endl;
    for(size_t i=10+alea_leq(10);i>0;--i)
    {
        dss.push( gen<string>::get() );
    }
    std::cerr << "dss.height=" << dss.height() << "/size=" << dss.size() << std::endl;
    for(ptrdiff_t i=ptrdiff_t(dss.height());i>0;--i)
    {
        std::cerr << i << " : " << dss[i] << std::endl;
    }
    std::cerr << std::endl;;
    for(ptrdiff_t i=-1;i>=-ptrdiff_t(dss.height());--i)
    {
        std::cerr << i << " : " << dss[i] << std::endl;
    }

    dss.clear();
    std::cerr << "dss.height=" << dss.height() << "/size=" << dss.size() << std::endl;


}
YOCTO_UNIT_TEST_DONE()
