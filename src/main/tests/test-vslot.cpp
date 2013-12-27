#include "yocto/utest/run.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/string.hpp"

using namespace yocto;

#define __SHOW() std::cerr << "name= <" << v1.name() << ">" << std::endl
#define __COPY() do { const vslot v2(v1); std::cerr << "copy= <" << v2.name() << ">" << std::endl; } while(false)

YOCTO_UNIT_TEST_IMPL(vslot)
{
    std::cerr << "sizeof(vslot)=" << sizeof(vslot) << std::endl;
    vslot v1;
    
    v1.allocate(2);    __SHOW(); __COPY();
    v1.make<float>();  __SHOW(); __COPY();
    v1.make<double>(); __SHOW(); __COPY();
    v1.release();      __SHOW();
    
    v1.make<short>(2);        std::cerr << v1.as<short>()  << std::endl; __SHOW(); __COPY();
    v1.make<float>(4);        std::cerr << v1.as<float>()  << std::endl; __SHOW(); __COPY();
    v1.make<double>(8);       std::cerr << v1.as<double>() << std::endl; __SHOW(); __COPY();
    v1.make<string>("hello"); std::cerr << v1.as<string>() << std::endl; __SHOW(); __COPY();
}
YOCTO_UNIT_TEST_DONE()
