#include "yocto/gems/library.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gems;

YOCTO_UNIT_TEST_IMPL(lib)
{
    std::cerr << "sizeof(library<float>)  = " << sizeof( library<float>  ) << std::endl;
    std::cerr << "sizeof(library<double>) = " << sizeof( library<double> ) << std::endl;

    library<float> lib;
    
    lib.declare_residue(0, "simu");
    lib.declare_atom(0, "H", 1);
    
    std::cerr << "type of simu = " <<   lib.residues.type_of("simu") << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()
