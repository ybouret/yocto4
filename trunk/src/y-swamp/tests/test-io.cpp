#include "yocto/utest/run.hpp"

#include "yocto/swamp/data-io.hpp"
#include "yocto/swamp/common.hpp"

#include "yocto/code/rand.hpp"


using namespace yocto;
using namespace swamp;


YOCTO_UNIT_TEST_IMPL(io)
{
    typedef array1D<double> Array1D;
    typedef array2D<double> Array2D;
    typedef array3D<double> Array3D;
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
        vector<double>  values(2,0);
        
        Array1D &A = D["A"].as<Array1D>();
        Array1D &B = D["B"].as<Array1D>();
        A.set_all(A,1);
        B.set_all(B,2);
        
        dataIO::query<double,Array1D>(values,ld,0);
        std::cerr << "values=" << values << std::endl;
        for( size_t i=D.in_layout.size(); i>0;--i)
        {
            const size_t j = D.in_layout[i];
            dataIO::query<double,Array1D>(values,ld,j);
            dataIO::store<double,Array1D>(ld,values,j);
        }
    }
    
    {
        
    }
    
    {
        
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
