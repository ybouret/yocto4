#include "yocto/utest/run.hpp"
#include "yocto/pack/huff.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/code/mtf.hpp"

using namespace yocto;
using namespace packing;


YOCTO_UNIT_TEST_IMPL(huff)
{
    ios::bitio    out;
    huffman::tree huff;
    
    huffman::tree       huff2;
    core::move_to_front mtf;
    ios::bitio          out2;

    
    ios::icstream fp( ios::cstdin );
    ios::ocstream bin(  "huff.dat", false );
    ios::ocstream bin2( "huff2.dat", false );

    string line;
    
    std::cerr << "enter data:" << std::endl;
    
    char C = 0;
    size_t num_in   = 0;
    size_t num_out  = 0;
    size_t num_out2 = 0;
    
    while( fp.query(C) )
    {
        ++num_in;
        huff.encode(out, C);
        huff2.encode(out2, mtf.encode(C) );
        while( out.size() >= 8 )
        {
            bin.write( out.pop_full<uint8_t>() );
            ++num_out;
        }
        
        while( out2.size() >= 8 )
        {
            bin2.write( out2.pop_full<uint8_t>() );
            ++num_out2;
        }
        
    }
    
    huff.flush( out );
    while( out.size() >= 8 )
    {
        bin.write( out.pop_full<uint8_t>() );
        ++num_out;
    }
    
    huff2.flush( out2 );
    while( out2.size() >= 8 )
    {
        bin2.write( out2.pop_full<uint8_t>() );
        ++num_out2;
    }

    std::cerr << "#in  = "  << num_in  << std::endl;
    std::cerr << "#out = "  << num_out << std::endl;
    std::cerr << "#out2 = " << num_out2 << std::endl;

    huff.show( std::cerr );
    huff2.show( std::cerr );
    huff.graph( "huff.viz" );
}
YOCTO_UNIT_TEST_DONE()
