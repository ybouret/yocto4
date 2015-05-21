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
        std::cerr << "-- compiling '" << expr << "'" << std::endl;
        auto_ptr<pattern> q( compile(expr,NULL)  );
        std::cerr << "-- saving expr.dot" << std::endl;
        q->graphviz("expr.dot");
        std::cerr << "-- rendering expr.png" << std::endl;
        system("dot -Tpng -o expr.png expr.dot");
        std::cerr << "-- detecting first chars" << std::endl;
        first_chars fc;
        q->detect(fc);
        std::cerr << "first_chars=[";
        for(size_t i=1;i<=fc.size();++i)
        {
            std::cerr << make_visible(fc[i]);
        }
        std::cerr << "] " << (fc.accept_empty  ? "ACCEPT EMPTY" : "NEVER EMPTY" ) << std::endl;
        
        std::cerr << "-- binary codes..." << std::endl;
        const string org = q->to_binary();
        auto_ptr<pattern> r( pattern::load(org) );
        const string str = r->to_binary();
        if(org!=str)
            throw exception("binary code mismatch");
        
        
    }
    
}
YOCTO_UNIT_TEST_DONE()
