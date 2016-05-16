#include "yocto/graphics/image/tiff++.hpp"
#include "yocto/utest/run.hpp"
#include <cstdio>

using namespace yocto;
using namespace graphics;

YOCTO_UNIT_TEST_IMPL(tiff)
{
    for(int i=1;i<argc;++i)
    {
        const string fn = argv[i];
        std::cerr << "-- loading " << fn << std::endl;
        I_TIFF tif(fn);
        I_TIFF::Raster raster;
        int count = 0;
        do
        {
            ++count;
            const int w = tif.GetWidth();
            const int h = tif.GetHeight();
            std::cerr << "size=" << w << "x" << h << std::endl;
            tif.ReadRBGAImage(raster);
        }
        while( tif.ReadDirectory() );
        std::cerr << "found " << count << " entry(ies)" << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()