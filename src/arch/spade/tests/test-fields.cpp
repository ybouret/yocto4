#include "yocto/utest/run.hpp"
#include "yocto/spade/fields/setup.hpp"

#include "./main.hpp"
#include "yocto/spade/array3d.hpp"


YOCTO_UNIT_TEST_IMPL(fields)
{
    array_db       db;
    
    {
        fields_setup<layout1D> F(8);
        
        Y_SPADE_FIELD(F, "A", array1D<double> );
        Y_SPADE_FIELD(F, "B", array1D<int>    );
        
        std::cerr << "Got " << F.size() << " fields" << std::endl;
        
        const layout1D L( RandCoord1D(), RandCoord1D() );
        F.create(L,db);
    }
    
    array1D<double> &A = db["A"].as< array1D<double> >();
    A.ldz();
    
    {
        fields_setup<layout3D> F(2);
        Y_SPADE_FIELD(F, "A3", array3D<double> );
        Y_SPADE_FIELD(F, "B3", array3D<size_t> );
        const layout3D L( RandCoord3D(), RandCoord3D() );

        F.create(L,db);
    }
    
    array3D<size_t> &B3 = db["B3"].as< array3D<size_t> >();
    std::cerr << "B3: " << B3.as_layout() << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

