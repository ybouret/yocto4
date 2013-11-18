#include "yocto/utest/run.hpp"
#include "yocto/aqueous/library.hpp"

using namespace yocto;
using namespace aqueous;

YOCTO_UNIT_TEST_IMPL(lib)
{
    
    library lib( 100 );
    lib.add( "H+",   1 );
    lib.add( "HO-", -1 );
    lib.ld<string,string>( "HA-", -1, "Hello" );
}
YOCTO_UNIT_TEST_DONE()
