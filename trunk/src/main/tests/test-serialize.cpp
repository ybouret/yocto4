#include "yocto/ios/serialize.hpp"
#include "yocto/utest/run.hpp"


using namespace yocto;

YOCTO_UNIT_TEST_IMPL(serialize)
{
    ios::serialize::items content(5);
    content.declare("sigma",8);
    content.declare("theta",2);

    std::cerr << "content.linear=" << content.linear << std::endl;
    std::cerr << "content.offset=" << content.offset << std::endl;

}
YOCTO_UNIT_TEST_DONE()

