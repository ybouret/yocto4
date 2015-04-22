#include "yocto/lang/regexp.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;
using namespace lang;

YOCTO_UNIT_TEST_IMPL(regexp)
{
    if(argc>1)
    {
        const string      expr = argv[1];
        auto_ptr<pattern> p( regexp(expr,NULL) );
        p->graphviz("rx.dot");
        (void) system("dot -Tpng rx.dot -o rx.png");
    }
}
YOCTO_UNIT_TEST_DONE()
