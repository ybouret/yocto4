#include "yocto/utest/run.hpp"
#include "yocto/lingua/syntax/xnode.hpp"

using namespace yocto;
using namespace lingua;


YOCTO_UNIT_TEST_IMPL(syntax)
{
    std::cerr << "sizeof(xnode)=" << sizeof(syntax::xnode) << std::endl;
}
YOCTO_UNIT_TEST_DONE()
