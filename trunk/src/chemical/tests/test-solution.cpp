#include "yocto/chemical/solution.hpp"
#include "yocto/utest/run.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(solution)
{
    chemical::collection lib;
    lib.reserve(16);
    
    lib.add("H+");
    lib.add("HO-");
    lib.add("Cl-");
    lib.add("Na+");

    lib.dump_ztable(std::cerr);
    
    chemical::solution S0(lib);
    std::cerr << "S0=" << S0 << std::endl;
    std::cerr << S0.components << " components" << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()

