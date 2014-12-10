#include "yocto/gems/frame.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace gems;

YOCTO_UNIT_TEST_IMPL(frame)
{
    frame<float> F;
    
    const char * res_name[] =
    {
        "res0", "res1", "res3"
    };
    
    const word_t res_num = sizeof(res_name)/sizeof(res_name[0]);
    
    for(word_t i=0;i<res_num;++i)
    {
        std::cerr << "residue type " << i << " => " << res_name[i] << std::endl;
        F.lib.declare_residue(i, res_name[i]);
    }
    
    const word_t nr = 1 + alea_leq(10);
    for(size_t i=nr;i>0;--i)
    {
        (void)F.add_residue(i%res_num);
    }
    
    const char *atm_name[] = { "C", "H", "O", "N" };
    const float atm_mass[] = { 12, 1, 16, 14 };
    const size_t atm_num = sizeof(atm_name)/sizeof(atm_name[0]);
    for(word_t i=0;i<atm_num;++i)
    {
        F.lib.declare_atom(i, atm_name[i], atm_mass[i]);
    }
    
    const size_t na = (nr+1) * (1+alea_leq(10));
    for(size_t i=0;i<na;++i)
    {
        const word_t ir = i % nr;
        const word_t ia = i % atm_num;
        F.add_atom_to(ir, ia);
    }
    
    
    std::cerr << "na=" << na << std::endl;
}
YOCTO_UNIT_TEST_DONE()
