#include "yocto/utest/run.hpp"
#include "yocto/pack/huffman.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include <cstdlib>

using namespace yocto;
using namespace packing;


YOCTO_UNIT_TEST_IMPL(huff)
{
    Huffman::Tree tree;
    ios::icstream in( ios::cstdin );
    ios::bitio    out;
    
    char C=0;
    while( in.query(C) )
    {
        tree.encode(out,uint8_t(C) );
    }
    
    {
        ios::ocstream fp("huff.dot",false);
        tree.graphviz("G", fp);
    }
    system( "dot -Tpng huff.dot -o huff.png" );
    {
        ios::ocstream fp( ios::cstderr );
        tree.display(fp);
    }
}
YOCTO_UNIT_TEST_DONE()
