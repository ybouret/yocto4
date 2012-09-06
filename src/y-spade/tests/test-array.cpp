#include "yocto/utest/run.hpp"
#include "./main.hpp"

#include "yocto/spade/array1d.hpp"
#include "yocto/spade/data-block.hpp"

YOCTO_UNIT_TEST_IMPL(array1D)
{
    const layout1D L1( RandCoord1D(), RandCoord1D());
    array1D<float> A1( L1 );
    std::cerr << "A1.items=" << A1.items << std::endl;
    std::cerr << "A1.bytes=" << A1.bytes << std::endl;
    std::cerr << "A1: " << A1.as_layout() << std::endl;
    data_block     B1(A1);
    
    A1.ldz();
    for( unit_t i=A1.lower; i <= A1.upper; ++i )
    {
        A1[i] = i;
    }
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/array2d.hpp"

YOCTO_UNIT_TEST_IMPL(array2D)
{
    const layout2D L2( RandCoord2D(), RandCoord2D());
    array2D<int>   A2( L2 );
    std::cerr << "A2.items=" << A2.items << std::endl;
    std::cerr << "A2.bytes=" << A2.bytes << std::endl;
    std::cerr << "A2: " << A2.as_layout() << std::endl;
    data_block     B2(A2);
    
    A2.ldz();
    for( unit_t j=A2.lower.y;j<=A2.upper.y;++j)
    {
        for( unit_t i=A2.lower.x; i <= A2.upper.x; ++i )
        {
            A2[j][i] = i*j;
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/array3d.hpp"

YOCTO_UNIT_TEST_IMPL(array3D)
{
    const layout3D   L3( RandCoord3D(), RandCoord3D());
    array3D<double>  A3( L3 );
    std::cerr << "A3.items=" << A3.items << std::endl;
    std::cerr << "A3.bytes=" << A3.bytes << std::endl;
    std::cerr << "A3: " << A3.as_layout() << std::endl;
}
YOCTO_UNIT_TEST_DONE()

