#include "yocto/tiff++/plugin.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(open)
{
    if(argc>1)
    {
        const string path = argv[1];
        const module tiff_dll( dylib_load(path.c_str(), dylib_full) );
        gfx::TIFF_Interface I(tiff_dll,"load_tiff");
    }
}
YOCTO_UNIT_TEST_DONE()
