#include "yocto/gems/particle.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gems;

YOCTO_UNIT_TEST_IMPL(particle)
{
    std::cerr << "sizeof(particle<float>)="  << sizeof( gems::particle<float> ) << std::endl;
    std::cerr << "sizeof(particle<double>)=" << sizeof( gems::particle<double>) << std::endl;

}
YOCTO_UNIT_TEST_DONE()
