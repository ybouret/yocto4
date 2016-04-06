#include "yocto/lingua/regexp/compiler.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/lingua/pattern/posix.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(regexp)
{
    p_dict dict;
    dict.define("DIGIT","[0-9]");
    dict.define("INT","{DIGIT}+");


    if(argc>1)
    {
        const string      expr = argv[1];
        auto_ptr<pattern> p( regexp::compile(expr,&dict) );
        p->graphviz("rx.dot");
        ios::graphviz_render("rx.dot");
        
    }
}
YOCTO_UNIT_TEST_DONE()
