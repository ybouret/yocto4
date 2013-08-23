#include "yocto/utest/run.hpp"
#include "yocto/gems/io/xyz.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace gems;


YOCTO_UNIT_TEST_IMPL(xyz)
{
    if( argc > 1)
    {
        const string  fn = argv[1];
        
        
        size_t nmax = 10;
        if( argc > 2)
            nmax = strconv::to_size( argv[2], "nmax");
        
        if(true)
        {
            XYZ<double>::Frames frames;
            ios::icstream fp(fn);
            frames.load( fp, nmax );
            std::cerr << "#frames=" << frames.size() << std::endl;
        }
        
    }
}
YOCTO_UNIT_TEST_DONE()

