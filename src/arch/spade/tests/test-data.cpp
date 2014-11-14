#include "yocto/utest/run.hpp"
#include "./main.hpp"
#include "yocto/spade/dataspace.hpp"
#include "yocto/spade/array1d.hpp"

YOCTO_UNIT_TEST_IMPL(data1D)
{
    const layout1D         L( RandCoord1D(), RandCoord1D() );
    fields_setup<layout1D> F(8);
    ghosts_setup           G;

    Y_SPADE_FIELD(F, "A", array1D<float> );
    Y_SPADE_FIELD(F, "B", array1D<int>   );

    
    dataspace<layout1D> ds(L,F,G);
       
    F.clear();
    
    std::cerr << "ds1.layout : " << ds.as_layout() << std::endl;
    std::cerr << "ds1.outline: " << ds.outline    << std::endl;
    
    array1D<float> &A = ds["A"].as< array1D<float> >();
    A.ldz();
    
    
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/array2d.hpp"

YOCTO_UNIT_TEST_IMPL(data2D)
{
    const layout2D         L( RandCoord2D(), RandCoord2D() );
    fields_setup<layout2D> F(8);
    ghosts_setup           G;

    Y_SPADE_FIELD(F, "A", array2D<float> );
    Y_SPADE_FIELD(F, "B", array2D<int>   );
    
    
    dataspace<layout2D> ds(L,F,G);
    
    F.clear();
    
    std::cerr << "ds2.layout : " << ds.as_layout() << std::endl;
    std::cerr << "ds2.outline: " << ds.outline    << std::endl;
    
    array2D<float> &A = ds["A"].as< array2D<float> >();
    A.ldz();
       
    
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/spade/array3d.hpp"

YOCTO_UNIT_TEST_IMPL(data3D)
{
    const layout3D         L( RandCoord3D(), RandCoord3D() );
    fields_setup<layout3D> F(8);
    ghosts_setup           G;

    Y_SPADE_FIELD(F, "A", array3D<float> );
    Y_SPADE_FIELD(F, "B", array3D<int>   );
    
    
    dataspace<layout3D> ds(L,F,G);
    
    F.clear();
    
    std::cerr << "ds3.layout : " << ds.as_layout() << std::endl;
    std::cerr << "ds3.outline: " << ds.outline    << std::endl;
    
    array3D<float> &A = ds["A"].as< array3D<float> >();
    A.ldz();
    
    
}
YOCTO_UNIT_TEST_DONE()

