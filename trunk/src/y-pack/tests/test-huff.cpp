#include "yocto/utest/run.hpp"
#include "yocto/pack/huffman.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include <cstdlib>

using namespace yocto;
using namespace packing;


YOCTO_UNIT_TEST_IMPL(huff)
{
    std::cerr << "sizeof(Huffman::Node)= " << sizeof(Huffman::Node) << std::endl;
    
    Huffman::Codec tree;
    ios::icstream in( ios::cstdin );
    ios::bitio    out;
    
    size_t       nIn = 0;
    size_t       nOut = 0;
    char C=0;
    ios::ocstream save( "huff.dat", false );
    while( in.query(C) )
    {
        ++nIn;
        tree.write(out,uint8_t(C) );
        while( out.size() >= 8 )
        {
            save.write( out.pop_full<uint8_t>() );
            ++nOut;
        }
    }
    
    tree.flush(out);
    while( out.size() >= 8 )
    {
        ++nOut;
        save.write( out.pop_full<uint8_t>() );
    }
    
    std::cerr << "bytes: " << nIn << " => " << nOut << std::endl;
    
    {
        ios::ocstream fp("huff.dot",false);
        tree.tree().graphviz("G", fp);
    }
    system( "dot -Tpng huff.dot -o huff.png" );
    {
        ios::ocstream fp( ios::cstderr );
        fp("Core Tree: \n");
        tree.tree().display(fp);
    }
}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(huffenc)
{
    Huffman::Codec tree;
    ios::icstream src( ios::cstdin );
    ios::ocstream tgt( ios::cstdout );
    ios::bitio    bio;
    size_t nIn = 0, nOut = 0;
    char C=0;
    while( src.query(C) )
    {
        ++nIn;
        tree.write(bio, C);
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
    Huffman::Codec tree;
    ios::icstream src( ios::cstdin );
    ios::ocstream tgt( ios::cstdout );
    ios::bitio    bio;
    
    Huffman::DecodeHandle  h;
    tree.decode_init(h);
    
    char C=0,D=0;
    while( src.query(C) )
    {
        bio.push_full<uint8_t>(C);
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




