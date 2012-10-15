#include "yocto/utest/run.hpp"
#include "yocto/mpk/natural.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace mpk;


YOCTO_UNIT_TEST_IMPL(io)
{
    string        line;
    ios::icstream in( ios::cstdin );
    while( (std::cerr << "> ").flush(), in.gets(line) )
    {
        const natural n = natural::dec(line);
        std::cerr << "\t HEX:" << std::hex << n << std::endl;
        std::cerr << "\t DEC:" << std::dec << n << std::endl;

    }
    
}
YOCTO_UNIT_TEST_DONE()
