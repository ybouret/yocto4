#include "yocto/utest/run.hpp"
#include "yocto/pack/huff.hpp"

using namespace yocto;
using namespace packing;


YOCTO_UNIT_TEST_IMPL(huff)
{
    
    huffman::tree huff;
    huff.show( std::cerr );
    huff.update('A');
    huff.show( std::cerr );
}
YOCTO_UNIT_TEST_DONE()
