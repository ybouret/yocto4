#include "yocto/pack/huffman.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(huff)
{

    pack::huffman::alphabet  Alpha;
    pack::huffman::tree_type Tree;
    
    ios::icstream fp( ios::cstdin );
    char C = 0;
    while( fp.query(C) )
    {
        Alpha.update(C);
    }
    Alpha.display(std::cerr);

    Tree.build_for(Alpha);
    Alpha.display(std::cerr);
    Tree.graphviz("huff.dot");
    ios::graphviz_render("huff.dot");
}
YOCTO_UNIT_TEST_DONE()