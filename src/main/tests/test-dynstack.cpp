#include "yocto/stock/dynstack.hpp"
#include "yocto/utest/run.hpp"
#include "support.hpp"

using namespace yocto;

namespace {

    template <typename T>
    inline void dump_ds(const dynstack_of<T> &d )
    {
        for(ptrdiff_t i=d.height();i>0;--i)
        {
            std::cerr << i << " : " << d[i] << std::endl;
        }
        std::cerr << std::endl;;

    }

}
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
    dss.new_frame();
    std::cerr << "dss.height=" << dss.height() << "/size=" << dss.size() << std::endl;
    for(size_t i=10+alea_leq(10);i>0;--i)
    {
        dss.push( gen<string>::get() );
    }
    std::cerr << "dss.height=" << dss.height() << "/size=" << dss.size() << std::endl;

    dump_ds(dss);
    for(ptrdiff_t i=-1;i>=-ptrdiff_t(dss.height());--i)
    {
        std::cerr << i << " : " << dss[i] << std::endl;
    }
    dss.clear();
    std::cerr << "dss.height=" << dss.height() << "/size=" << dss.size() << std::endl;

    std::cerr << std::endl;
    std::cerr << "-- testing multiple frames..." << std::endl;
    dss.release();
    std::cerr << "height=" << dss.height() << ", size=" << dss.size() << ", maxi=" << dss.capacity() << std::endl;


    std::cerr << "---- main frame" << std::endl;
    for(size_t i=5+alea_leq(10);i>0;--i)
    {
        dss.push( gen<string>::get() );
    }
    dump_ds(dss);
    size_t num_frames=0;
    for(size_t nf=2+alea_leq(5);nf>0;--nf)
    {
        dss.new_frame();
        ++num_frames;
        std::cerr << "---- frame #" << dss.frames() << "/" << num_frames << std::endl;
        for(size_t i=5+alea_leq(10);i>0;--i)
        {
            dss.push( gen<string>::get() );
        }
        dump_ds(dss);
    }
    std::cerr << "#frames=" << dss.frames() << std::endl;

    while(dss.frames()>0)
    {
        std::cerr << "---- output frame #" << dss.frames() << std::endl;
        std::cerr << "height=" << dss.height() << ", size=" << dss.size() << ", maxi=" << dss.capacity() << std::endl;
        dump_ds(dss);
        dss.pop_frame();
    }
    std::cerr << "---- output main frame" << std::endl;
    dump_ds(dss);
}
YOCTO_UNIT_TEST_DONE()
