#include "yocto/utest/run.hpp"
#include "./main.hpp"
#include "yocto/spade/array1d.hpp"
#include "yocto/spade/fields-setup.hpp"

YOCTO_UNIT_TEST_IMPL(fields)
{
    array_db db;
    
    fields_setup<layout1D> F(8);
    
    Y_SPADE_FIELD(F, "A", array1D<double> );
    Y_SPADE_FIELD(F, "B", array1D<int>    );
    
    std::cerr << "Got " << F.size() << " fields" << std::endl;
    
    const layout1D L( RandCoord1D(), RandCoord1D() );
    F.create(L,db);
    
    
    array1D<double> &A = db["A"].as< array1D<double> >();
    A.ldz();
    
}
YOCTO_UNIT_TEST_DONE()

