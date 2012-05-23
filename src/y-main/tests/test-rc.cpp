#include "yocto/utest/run.hpp"
#include "yocto/ios/rc.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(rcpack)
{
    if( argc > 1 )
    {
        const string filename = argv[1];
        error_type   status   = 0;
        {
            ios::resources::packer rc( filename, status);
            for( int i=2; i < argc; ++i )
            {
                const string rcname = argv[i];
                std::cerr << "packing: " << rcname << std::endl;
                rc( rcname, rcname);
            }
        }
    }
}
YOCTO_UNIT_TEST_DONE()



YOCTO_UNIT_TEST_IMPL(rcload)
{
    if( argc > 1 )
    {
        const string   filename = argv[1];
        ios::resources rc(filename);
    }

}
YOCTO_UNIT_TEST_DONE()
