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
    
    size_t       nIn = 0;
    size_t       nOut = 0;
    char C=0;
    ios::ocstream save( "huff.dat", false );
    while( in.query(C) )
    {
        ++nIn;
        tree.encode(out,uint8_t(C) );
        while( out.size() >= 8 )
        {
            save.write( out.pop_full<uint8_t>() );
            ++nOut;
        }
    }
    if( out.size() > 0 )
    {
        ++nOut;
        save.write( out.pop<uint8_t>(out.size()) );
    }
    
    std::cerr << "bytes: " << nIn << " => " << nOut << std::endl;
    
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
