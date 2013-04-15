#include "yocto/utest/run.hpp"
#include "yocto/gems/io/xyz.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace gems;


YOCTO_UNIT_TEST_IMPL(xyz)
{
    if( argc > 1)
    {
        const string  fn = argv[1];
        ios::icstream fp(fn);
        
        IO::XYZ::Frames frames;
        
        frames.load( fp, 10 );
        std::cerr << "#frames=" << frames.size() << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()

