#include "yocto/utest/run.hpp"
#include "yocto/lang/compiler.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/ocstream.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lang;



YOCTO_UNIT_TEST_IMPL(comp_ast)
{
    compiler C;
    ios::icstream fp( ios::cstdin );
    auto_ptr<syntax::parse_node> Tree( C(fp) );
    if( Tree.is_valid() )
    {
        {
            ios::ocstream out( "c.dot", false );
            Tree->graphviz("G", out);
        }
        std::cerr << "Graphviz..." << std::endl;
        system("dot -Tpng c.dot -o c.png");
    }
}
YOCTO_UNIT_TEST_DONE()
