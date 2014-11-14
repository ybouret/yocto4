#include "yocto/chemical/species.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(species)
{
    species sp("H+",1);
    sp.data.make<double>(1.0);
    
    species::pointer sp1( new species("HO-",-1) );
    
    
}
YOCTO_UNIT_TEST_DONE()
