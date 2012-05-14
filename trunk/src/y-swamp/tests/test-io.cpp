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
        std::cerr << "dataIO: in 1D" << std::endl;
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
        std::cerr << "dataIO: in 2D" << std::endl;
        fields_setup<layout2D>      F;
        Y_SWAMP_DECL_VAR(F, "B", Array2D );
        Y_SWAMP_DECL_VAR(F, "A", Array2D );
        const layout2D              L(coord2D(-10,-10),coord2D(10,10));
        const ghosts_setup<coord2D> G;
        dataspace<layout2D>         D(L,G,F);
        vector<varray*>             ld;
        variables                   var;
        var << "A" << "B";
        dataIO::make_loader(ld, D, var);
        vector<double>  values(2,0);
        
        Array2D &A = D["A"].as<Array2D>();
        Array2D &B = D["B"].as<Array2D>();
        A.set_all(A,1);
        B.set_all(B,2);
        
        dataIO::query<double,Array2D>(values,ld,0);
        std::cerr << "values=" << values << std::endl;
        for( size_t i=D.in_layout.size(); i>0;--i)
        {
            const size_t j = D.in_layout[i];
            dataIO::query<double,Array2D>(values,ld,j);
            dataIO::store<double,Array2D>(ld,values,j);
        }

    }
    
    { 
        std::cerr << "dataIO: in 3D" << std::endl;
        fields_setup<layout3D>      F;
        Y_SWAMP_DECL_VAR(F, "B", Array3D );
        Y_SWAMP_DECL_VAR(F, "A", Array3D );
        const layout3D              L(coord3D(-10,-10,-10),coord3D(10,10,10));
        const ghosts_setup<coord3D> G;
        dataspace<layout3D>         D(L,G,F);
        vector<varray*>             ld;
        variables                   var;
        var << "A" << "B";
        dataIO::make_loader(ld, D, var);
        vector<double>  values(2,0);
        
        Array3D &A = D["A"].as<Array3D>();
        Array3D &B = D["B"].as<Array3D>();
        A.set_all(A,1);
        B.set_all(B,2);
        
        dataIO::query<double,Array3D>(values,ld,0);
        std::cerr << "values=" << values << std::endl;
        for( size_t i=D.in_layout.size(); i>0;--i)
        {
            const size_t j = D.in_layout[i];
            dataIO::query<double,Array3D>(values,ld,j);
            dataIO::store<double,Array3D>(ld,values,j);
        }

        
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
