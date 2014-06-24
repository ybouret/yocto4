#include "yocto/chemical/collection.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(collection)
{
   
    collection lib;
    lib.add("H+",1);
    lib.add("HO-",-1);
    
    std::cerr << lib << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
