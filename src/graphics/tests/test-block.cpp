#include "yocto/graphics/region.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace graphics;

YOCTO_UNIT_TEST_IMPL(block)
{
    std::cerr << "sizeof(vertex)=" << sizeof(vertex) << std::endl;
    std::cerr << "sizeof(displacement)=" << sizeof(displacement) << std::endl;
    std::cerr << "sizeof(region::node_type)=" << sizeof(region::node_type) << std::endl;
}
YOCTO_UNIT_TEST_DONE()
