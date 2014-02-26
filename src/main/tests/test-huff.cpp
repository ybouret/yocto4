#include "yocto/utest/run.hpp"
#include "yocto/pack/huffman.hpp"


using namespace yocto;
using namespace pack;

YOCTO_UNIT_TEST_IMPL(huff)
{
    std::cerr << "sizeof(Huffman::Item)=" << sizeof(Huffman::Item) << std::endl;
    
    Huffman::Alphabet alpha;
    
}
YOCTO_UNIT_TEST_DONE()
