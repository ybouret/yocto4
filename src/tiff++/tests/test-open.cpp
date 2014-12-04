#include "yocto/tiff++/io.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(open)
{
    if(argc>1)
    {
        const string path = argv[1];
        gfx::tiff_reader  tif(path);
        std::cerr << "#num=" << tif.num << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
