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
    
#if 0
    Huffman::Alphabet alpha;
    Huffman::Tree     tree;
    size_t            bytes=0;
    ios::bitio        bio;
    
    // first tree
    alpha.reset();
    tree.build_for(alpha);
    
    size_t emitted = 0;
    while( input.query(C) )
    {
        ++bytes;
        if(alpha[C].Freq<=0)
        {
            alpha[ Huffman::NYT ].emit(bio);
        }
        alpha[C].emit(bio);
        alpha.increase(C);
        tree.build_for(alpha);
        emitted += bio.size();
        bio.free();
    }
    alpha[ Huffman::END ].emit(bio);
    emitted += bio.size();
    bio.free();
    
    std::cerr << alpha << std::endl;
    std::cerr << "input:  " << bytes*8 << std::endl;
    std::cerr << "output: " << emitted << std::endl;
#endif
    
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
        H2.write( mtf.encode(C) );
        while( H1.query(C) )
        {
            fp1.write(C);
            ++n1;
        }
        
        while( H1.query(C) )
        {
            fp2.write(C);
            ++n2;
        }
        
    }
    H1.flush();
    H2.flush();
    while( H1.query(C) )
    {
        fp1.write(C);
        ++n1;
    }
    while( H2.query(C) )
    {
        fp2.write(C);
        ++n2;
    }

    std::cerr << "#input   = " << ni << std::endl;
    std::cerr << "#output1 = " << n1 << std::endl;
    std::cerr << "#output2 = " << n2 << std::endl;
}
YOCTO_UNIT_TEST_DONE()
