#include "yocto/gems/atom.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gems;

YOCTO_UNIT_TEST_IMPL(atom)
{
    std::cerr << "sizeof(atom<float>)="  << sizeof( gems::atom<float> ) << std::endl;
    std::cerr << "sizeof(atom<double>)=" << sizeof( gems::atom<double>) << std::endl;

}
YOCTO_UNIT_TEST_DONE()
