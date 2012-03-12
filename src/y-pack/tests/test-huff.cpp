#include "yocto/utest/run.hpp"
#include "yocto/pack/huff.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace packing;


YOCTO_UNIT_TEST_IMPL(huff)
{
    ios::bitio    out;
    huffman::tree huff;
    huff.show( std::cerr );
    
    ios::icstream fp( ios::cstdin );
    ios::ocstream bin( "huff.dat", false );
    string line;
    
    std::cerr << "enter data:" << std::endl;
    
    char C = 0;
    size_t num_in = 0, num_out = 0;
    while( fp.query(C) )
    {
        ++num_in;
        huff.encode(out, C);
        while( out.size() >= 8 )
        {
            bin.write( out.pop_full<uint8_t>() );
            ++num_out;
        }
    }
    
    huff.flush( out );
    while( out.size() >= 8 )
    {
        bin.write( out.pop_full<uint8_t>() );
        ++num_out;
    }
    std::cerr << "#in  = " << num_in  << std::endl;
    std::cerr << "#out = " << num_out << std::endl;
    huff.show( std::cerr );
    huff.graph( "huff.viz" );
}
YOCTO_UNIT_TEST_DONE()
