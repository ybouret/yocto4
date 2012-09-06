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
