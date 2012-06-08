#include "yocto/utest/run.hpp"
#include "yocto/lang/compiler.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/ios/ocstream.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lang;



YOCTO_UNIT_TEST_IMPL(compiler)
{
    compiler C;
    ios::icstream fp( ios::cstdin );
    auto_ptr<syntax::parse_node> Tree( C(fp) );
    if( Tree.is_valid() )
    {
        
    }
}
YOCTO_UNIT_TEST_DONE()
