#include "yocto/utest/run.hpp"
#include "yocto/aqueous/lua.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace aqueous;



YOCTO_UNIT_TEST_IMPL(lua)
{
    
    
    library lib;
    
    lib.add( "H+", 1 );
    lib.add( "HO-", -1 );
    lib.add( "AcH", 0 );
    lib.add( "Ac-", -1);
    
    solution s(lib);
    
    
}
YOCTO_UNIT_TEST_DONE()
