#include "yocto/utest/run.hpp"
#include "yocto/hashing/rmd.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(rmd)
{
    
    hashing::rmd  RMD;
    ios::icstream fp( ios::cstdin );
    char C = 0;
    while( fp.query(C) )
    {
        if( RMD.store(C) )
        {
            
        }
    }
    const size_t nleft = RMD.length & 63;
    std::cerr << "nleft=" << nleft << std::endl;
    const uint8_t *b = RMD.flush();
    std::cerr << "[";
    for( size_t i=0; i < nleft; ++i)
        std::cerr << char(b[i]);
    std::cerr << "]" << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

