#include "yocto/gems/library.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gems;

YOCTO_UNIT_TEST_IMPL(lib)
{
    std::cerr << "sizeof(library<float>)  = " << sizeof( library<float>  ) << std::endl;
    std::cerr << "sizeof(library<double>) = " << sizeof( library<double> ) << std::endl;

    //library<float> lib;
}
YOCTO_UNIT_TEST_DONE()
