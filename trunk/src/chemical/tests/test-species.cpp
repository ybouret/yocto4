#include "yocto/chemical/species.hpp"
#include "yocto/utest/run.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(species)
{
    chemical::species Proton("H+",1);
    
    Proton.data.make<double>(100.0);
    
    
}
YOCTO_UNIT_TEST_DONE()

