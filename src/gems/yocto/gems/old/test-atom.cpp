#include "yocto/gems/atom.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(atom)
{
    std::cerr << "sizeof(atom<float> )=" << sizeof(gems::atoms<float>)  << std::endl;
    std::cerr << "sizeof(atom<double>)=" << sizeof(gems::atoms<double>) << std::endl;

    gems::atoms<double> p(1000);
    gems::atoms<float>  pf(1000);

    gems::library<double> lib;
    lib.append("H", 0, 1);
    
    p.append(0, lib["H"] );
    
    
}
YOCTO_UNIT_TEST_DONE()
