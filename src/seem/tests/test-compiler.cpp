#include "yocto/utest/run.hpp"
#include "yocto/seem/compiler.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/ios/graphviz.hpp"
#include "yocto/lingua/syntax/analyzer.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(compiler)
{
    Seem::Compiler           compiler( true );
    ios::icstream            fp(ios::cstdin);


    Seem::vCode tree = compiler.compile(fp);

    tree->graphviz(       compiler.gram->name + "_output.dot" );
    ios::graphviz_render( compiler.gram->name + "_output.dot" );

    lingua::syntax::analyzer walker( * compiler.gram );
    ios::ocstream  output( ios::cstderr );
    walker.walk(tree.__get(), &output);

}
YOCTO_UNIT_TEST_DONE()
