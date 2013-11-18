#include "yocto/utest/run.hpp"
#include "yocto/jpeg/jpeglib.h"

using namespace yocto;

#define __SHOW(TYPE) std::cerr << "sizeof(" #TYPE ") = " << sizeof(TYPE) << std::endl
YOCTO_UNIT_TEST_IMPL(imgfmt)
{
    __SHOW(jpeg_compress_struct);
    __SHOW(jpeg_decompress_struct);
    __SHOW(jpeg_error_mgr);
}
YOCTO_UNIT_TEST_DONE()
