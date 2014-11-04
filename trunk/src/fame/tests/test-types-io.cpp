#include "yocto/fame/types-io.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace fame;

YOCTO_UNIT_TEST_IMPL(tio)
{
    double  x = 100;
    uint8_t buf[12];
    double  y = 0;
    
    void *px = &x;
    void *out = buf;
    type_io<double>::put(out,px);
    
    void       *py = &y;
    void       *in = buf;
    type_io<double>::put(py,in);
    std::cerr << "y=" << y << std::endl;
}
YOCTO_UNIT_TEST_DONE()

