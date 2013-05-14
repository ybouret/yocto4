#include "yocto/utest/run.hpp"
#include "yocto/lingua/pattern/compiler.hpp"
#include "yocto/lingua/pattern/posix.hpp"

#include "yocto/auto-ptr.hpp"

#include <cstdlib>

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(rx)
{
    if(argc>1)
    {
        p_dict dict;
        dict.insert("DIGIT", posix::digit() );
        const string      rxp = argv[1];
        auto_ptr<pattern> q( compile(rxp,0) );
        
        q->graphviz( "expr.dot" );
        system("dot -Tpng -oexpr.png expr.dot");
    }
}
YOCTO_UNIT_TEST_DONE()
