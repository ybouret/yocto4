#include "yocto/utest/run.hpp"
#include "matte.h"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(matte)
{
    MakeWindowMatte()->show(argc,argv);
    
    (void)Fl::run();
}
YOCTO_UNIT_TEST_DONE()
