#include "yocto/lang/pattern/compiler.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;
using namespace lang;

YOCTO_UNIT_TEST_IMPL(compiler)
{
    
    if(argc>1)
    {
        p_dict dict;
        const string expr = argv[1];
        std::cerr << "compiling '" << expr << "'" << std::endl;
        auto_ptr<pattern> q( compile(expr,NULL)  );
        q->graphviz("expr.dot");
    }
    
}
YOCTO_UNIT_TEST_DONE()
