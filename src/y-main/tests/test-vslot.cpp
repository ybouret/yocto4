#include "yocto/utest/run.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/string.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(vslot)
{
    vslot v1;
    
    v1.allocate(2);
    
    v1.make<float>();
    v1.make<double>();

    v1.release();
    
    v1.make<short>(2);  std::cerr << v1.as<short>()  << std::endl;
    v1.make<float>(4);  std::cerr << v1.as<float>()  << std::endl;
    v1.make<double>(8); std::cerr << v1.as<double>() << std::endl;
    v1.make<string>("hello"); std::cerr << v1.as<string>() << std::endl;
}
YOCTO_UNIT_TEST_DONE()
