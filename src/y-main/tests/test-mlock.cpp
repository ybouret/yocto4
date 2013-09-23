#include "yocto/utest/run.hpp"
#include "yocto/memory/locked.hpp"


using namespace yocto;

YOCTO_UNIT_TEST_IMPL(mlock)
{
    memory::locked &vmem = memory::locked::instance();
    std::cerr << "using " << vmem.name << std::endl;
    std::cerr << "page size=" << vmem.page_size << std::endl;
}
YOCTO_UNIT_TEST_DONE()
