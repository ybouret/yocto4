#include "yocto/utest/run.hpp"
#include "yocto/pack/huffman.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/pack/mtf.hpp"

using namespace yocto;
using namespace pack;

YOCTO_UNIT_TEST_IMPL(huff)
{
    std::cerr << "sizeof(Huffman::Item)=" << sizeof(Huffman::Item) << std::endl;
    
    ios::icstream     input( ios::cstdin );
    char C = 0;
    
    const bool use2 = false;
    
    Huffman::encoder H1;
    Huffman::encoder H2;
    
    move_to_front    mtf;
    ios::ocstream fp1("huff1.dat",false);
    ios::ocstream fp2("huff2.dat",false);
    
    size_t ni = 0;
    size_t n1 = 0;
    size_t n2 = 0;
    while( input.query(C) )
    {
        ++ni;
        H1.write(C);
        if(use2)
        {
            H2.write( mtf.encode(C) );
        }
        while( H1.query(C) )
        {
            fp1.write(C);
            ++n1;
        }
        if(use2)
        {
            while( H2.query(C) )
            {
                fp2.write(C);
                ++n2;
            }
        }
        
    }
    H1.flush();
    while( H1.query(C) )
    {
        fp1.write(C);
        ++n1;
    }
    
    if(use2)
    {
        H2.flush();
        while( H2.query(C) )
        {
            fp2.write(C);
            ++n2;
        }
    }
    
    std::cerr << "#input   = " << ni << std::endl;
    std::cerr << "#output1 = " << n1 << std::endl;
    if(use2)
    {
        std::cerr << "#output2 = " << n2 << std::endl;
    }
    
    std::cerr << "H1:" << std::endl;
    std::cerr << H1.alphabet() << std::endl;
    
    if(use2)
    {
        std::cerr << "H2:" << std::endl;
        std::cerr << H2.alphabet() << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(unhuff)
{
    
    ios::icstream     input( ios::cstdin );
    ios::ocstream     output( ios::cstdout);
    char C = 0;
    Huffman::decoder  H;
    while( input.query(C) )
    {
        H.write(C);
        while(H.query(C))
        {
            output.write(C);
        }
    }
    H.flush();
    while(H.query(C))
    {
        output.write(C);
    }
}
YOCTO_UNIT_TEST_DONE()
