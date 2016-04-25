#include "yocto/lingua/parser.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace lingua;

YOCTO_UNIT_TEST_IMPL(gen)
{
    auto_ptr<parser> P;
    if(argc>1)
    {
        {
            const string  filename = argv[1];
            ios::icstream fp(filename);
            P.reset( parser::generate(fp,true) );
        }

        {
            ios::icstream fp( ios::cstdin );
            auto_ptr<syntax::xnode> tree( P->parse(fp) );
            tree->graphviz( P->grammar::name + "_output.dot" );
            ios::graphviz_render( P->grammar::name + "_output.dot" );
        }
    }



}
YOCTO_UNIT_TEST_DONE()
