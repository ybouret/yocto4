#include "yocto/gems/frame.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gems;

YOCTO_UNIT_TEST_IMPL(frame)
{
    frame<float> F;
    
    F.lib.declare_residue(0, "simu");
    F.add_residue(0);
    
    F.lib.declare_atom(0, "H", 1);
    F.add_atom_to(0,0);
    //F.add_atom_to(0,0);
    
}
YOCTO_UNIT_TEST_DONE()
