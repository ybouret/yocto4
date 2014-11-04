#include "yocto/fame/types-io.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace fame;

YOCTO_UNIT_TEST_IMPL(tio)
{
    double  x = 100;
    uint8_t buf[100];
    double  y = 0;
    
    const void *px = &x;
    void       *out = buf;
    type_io<double>::read(out,px);
    
    void       *py = &y;
    const void *in = buf;
    type_io<double>::read(py,in);
    std::cerr << "y=" << y << std::endl;
}
YOCTO_UNIT_TEST_DONE()

