#include "yocto/lingua/regexp/compiler.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(regexp)
{
    if(argc>1)
    {
        const string      expr = argv[1];
        auto_ptr<pattern> p( regexp::compile(expr) );
        p->graphviz("rx.dot");
        ios::graphviz_render("rx.dot",false);
    }
}
YOCTO_UNIT_TEST_DONE()
