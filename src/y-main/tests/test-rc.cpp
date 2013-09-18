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


#include "yocto/ptr/auto.hpp"

YOCTO_UNIT_TEST_IMPL(rcload)
{
    if( argc > 1 )
    {
        const string   filename = argv[1];
        ios::resources rc(filename);
        for( int i=2; i < argc; ++i )
        {
            auto_ptr<ios::ichannel> in( rc.load_channel( argv[i] ) );
            auto_ptr<ios::istream>  fp( rc.load_stream( argv[i] ) );
            char C = 0;
            std::cerr << "<" << argv[i] << ">" << std::endl;
            size_t count = 0;
            while( fp->query(C) )
            {
                std::cerr << C;
                ++count;
            }
            std::cerr << "</" << argv[i] << " count=" << count << " >" << std::endl;
        }
    }

}
YOCTO_UNIT_TEST_DONE()
