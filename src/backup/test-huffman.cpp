#include "yocto/pack/huffman.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/pack/mtf.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(huff)
{

    bool use_mtf = false;
    if(argc>1)
    {
        const string arg = argv[1];
        if("-mtf"==arg)
        {
            use_mtf = true;
        }
    }

    size_t num_inp = 0;
    size_t num_out = 0;

    ios::icstream src(ios::cstdin);
    ios::ocstream dst(ios::cstdout);
    pack::huffman::encoder HUF;
    pack::move_to_front    MTF;

    char C = 0;
    while( src.query(C) )
    {
        ++num_inp;
        if(0 == num_inp%1024)
        {
            std::cerr << "+";
        }
        if(0== num_inp%(1024*1024))
        {
            std::cerr << "[*]" << std::endl;
        }

        if(use_mtf) C=MTF.encode(C);
        HUF.write(C);
        while( HUF.query(C) )
        {
            ++num_out;
            dst.write(C);
        }
    }
    HUF.flush();
    while(HUF.query(C))
    {
        ++num_out;
        dst.write(C);
    }

    std::cerr << std::endl;
    std::cerr << "inp=" << num_inp << "/out=" << num_out << std::endl;

}
YOCTO_UNIT_TEST_DONE()