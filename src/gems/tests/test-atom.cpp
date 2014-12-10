#include "yocto/gems/atom.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(atom)
{
    std::cerr << "sizeof(atom<float> )=" << sizeof(gems::atoms<float>)  << std::endl;
    std::cerr << "sizeof(atom<double>)=" << sizeof(gems::atoms<double>) << std::endl;

    gems::atoms<double> p(1000);
    
}
YOCTO_UNIT_TEST_DONE()
