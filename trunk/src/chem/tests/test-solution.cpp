#include "yocto/chemical/solution.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(solution)
{
    collection lib;
    lib.add("H+",1);
    lib.add("HO-",-1);
    
    solution   S(lib);
    std::cerr << S << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
