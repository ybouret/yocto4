#include "yocto/script/parser.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(parser)
{
    Script::Parser p;
    ios::icstream  input( ios::cstdin );
    auto_ptr<Script::Node> ast( p.getAST(input) );
    if(ast.is_valid())
    {
        ast->graphviz("ast.dot");
        ios::graphviz_render("ast.dot");
    }
}
YOCTO_UNIT_TEST_DONE()

