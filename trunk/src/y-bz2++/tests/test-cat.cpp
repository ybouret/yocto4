#include "yocto/bz2++/bz2stream.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"


using namespace yocto;

YOCTO_UNIT_TEST_IMPL(cat)
{
    
    bzlib::ibz2stream *bzf = 0;
    if( argc > 0 )
        bzf = new bzlib::ibz2stream( argv[1] );
    else
        bzf = new bzlib::ibz2stream( ios::cstdin );
    
    auto_ptr<bzlib::ibz2stream> bz( bzf );
    string line;
    while( bz->read_line(line) >= 0 )
    {
        std::cout << line << std::endl;
        line.clear();
    }
    
}
YOCTO_UNIT_TEST_DONE()
