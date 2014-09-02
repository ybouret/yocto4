#include "yocto/lang/pattern/compiler.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"

using namespace yocto;
using namespace lang;
#include <cstdlib>

YOCTO_UNIT_TEST_IMPL(compiler)
{
    
    if(argc>1)
    {
        p_dict dict;
        const string expr = argv[1];
        std::cerr << "compiling '" << expr << "'" << std::endl;
        auto_ptr<pattern> q( compile(expr,NULL)  );
        std::cerr << "saving expr.dot" << std::endl;
        q->graphviz("expr.dot");
        std::cerr << "rendering expr.png" << std::endl;
        system("dot -Tpng -o expr.png expr.dot");
        
    }
    
}
YOCTO_UNIT_TEST_DONE()
