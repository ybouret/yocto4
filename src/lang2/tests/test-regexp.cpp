#include "yocto/lang/regexp.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"

#include <cstdlib>

using namespace yocto;
using namespace lang;

YOCTO_UNIT_TEST_IMPL(regexp)
{
    p_dict dict;
    dict.define("INT", regexp("(0|1|2|3|4|5|6|7|8|9)+",NULL) );
    
    if(argc>1)
    {
        const string      expr = argv[1];
        auto_ptr<pattern> p( regexp(expr,&dict) );
        p->graphviz("rx.dot");
        (void) system("dot -Tpng rx.dot -o rx.png");
    }
}
YOCTO_UNIT_TEST_DONE()
