#include "yocto/utest/run.hpp"
#include "yocto/seem/types.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/ios/graphviz.hpp"
#include "yocto/lingua/syntax/tree-walker.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(parser)
{
    Seem::Parser             P( true );
    ios::icstream            fp(ios::cstdin);

    auto_ptr<Seem::XNode> tree( P.impl->parse(fp) );
    tree->graphviz(       P.gram->name + "_output.dot" );
    ios::graphviz_render( P.gram->name + "_output.dot" );

    Seem::TreeWalker walker(*P.gram);
    std::cerr << "-- WALKING..." << std::endl;
    ios::ocstream output( ios::cstderr );
    walker.walk(tree.__get(), &output);

}
YOCTO_UNIT_TEST_DONE()
