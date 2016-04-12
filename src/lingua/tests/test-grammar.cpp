#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(grammar)
{
    std::cerr << "sizeof(xnode)=" << sizeof(syntax::xnode) << std::endl;
    std::cerr << "#U64=" << YOCTO_U64_FOR_ITEM(syntax::xnode::list_type) << std::endl;
}
YOCTO_UNIT_TEST_DONE()
