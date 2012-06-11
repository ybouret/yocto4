#include "yocto/utest/run.hpp"
#include "matte.h"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(matte)
{
    MakeWindowMatte()->show(argc,argv);
    
    return Fl::run();
}
YOCTO_UNIT_TEST_DONE()
