#include "yocto/utest/run.hpp"

#include "yocto/swamp/data-io.hpp"
#include "yocto/swamp/common.hpp"

#include "yocto/code/rand.hpp"


using namespace yocto;
using namespace swamp;


YOCTO_UNIT_TEST_IMPL(io)
{
    typedef array1D<double> Array1D;
    {
        fields_setup<layout1D>      F;
        Y_SWAMP_DECL_VAR(F, "B", Array1D );
        Y_SWAMP_DECL_VAR(F, "A", Array1D );
        const layout1D              L(-10,10);
        const ghosts_setup<coord1D> G;
        dataspace<layout1D>         D(L,G,F);
        vector<varray*>             ld;
        variables                   var;
        var << "A" << "B";
        dataIO::make_loader(ld, D, var);
        
        dataIO::process<double,Array1D>(ld,D);
        
    }
    
    {
        
    }
    
    {
        
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
