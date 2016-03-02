#include "yocto/utest/driver.hpp"

#include <cmath>
#include <cfloat>

YOCTO_UNIT_TEST_INIT(16)
{
    //const float sfac = (std::sqrt( -2.0f*std::log(FLT_EPSILON)));
    //std::cerr << "sfac=" << sfac << std::endl;
    
    YOCTO_UNIT_TEST_DECL(bitmap);
    YOCTO_UNIT_TEST_DECL(pixmap);
    YOCTO_UNIT_TEST_DECL(img);
    YOCTO_UNIT_TEST_DECL(types);
    YOCTO_UNIT_TEST_DECL(ops);
    YOCTO_UNIT_TEST_DECL(tiff2png);
    YOCTO_UNIT_TEST_DECL(block);
    YOCTO_UNIT_TEST_DECL(circle);
    
}
YOCTO_UNIT_TEST_EXEC()
