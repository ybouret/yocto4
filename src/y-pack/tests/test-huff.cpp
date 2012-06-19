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


YOCTO_UNIT_TEST_IMPL(huffenc)
{
    Huffman::Tree tree;
    ios::icstream src( ios::cstdin );
    ios::ocstream tgt( ios::cstdout );
    ios::bitio    bio;
    size_t nIn = 0, nOut = 0;
    char C=0;
    while( src.query(C) )
    {
        ++nIn;
        tree.encode(bio, C);
        while( bio.size() >= 8 )
        {
            ++nOut;
            tgt.write( bio.pop_full<uint8_t>() );
        }
    }
    tree.flush(bio);
    while( bio.size() > 0 )
    {
        const uint8_t b = bio.pop_full<uint8_t>();
        tgt.write( b );
        ++nOut;
    }
    std::cerr << "input: " << nIn << " => output: " << nOut << std::endl;

}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(huffdec)
{
    Huffman::Tree tree;
    ios::icstream src( ios::cstdin );
    ios::ocstream tgt( ios::cstdout );
    ios::bitio    bio;
    
    Huffman::DecodeHandle  h;
    tree.decode_init(h);
    
    char C=0,D=0;
    while( src.query(C) )
    {
        bio.push_full<uint8_t>(C);
        //bio.output(std::cerr << "bio:", bio.size());std::cerr << std::endl;

        for(;;)
        {
            const Huffman::DecodeStatus status = tree.decode(h, bio, D);
            if( status == Huffman::DecodePending )
                break;
            
            if( status == Huffman::DecodeSuccess )
            {
                tgt.write(D);
                continue;
            }
            
            if( status == Huffman::DecodeFlushed)
            {
                goto DONE;
            }
        }
        
    }
DONE:;
    
}
YOCTO_UNIT_TEST_DONE()




