#include "yocto/utest/run.hpp"
#include "yocto/pack/huff.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace packing;


YOCTO_UNIT_TEST_IMPL(huff)
{
    
    huffman::tree huff;
    huff.show( std::cerr );
    
    ios::icstream fp( ios::cstdin );
    string line;
    
    std::cerr << "enter data:" << std::endl;
    while( fp.read_line( line ) > 0 && line != ".quit" )
    {
        for( size_t i=0; i < line.size(); ++i )
            huff.update( line[i] );
        line.clear();
        huff.show( std::cerr );
        huff.graph( "huff.viz" );
    }
    
}
YOCTO_UNIT_TEST_DONE()
