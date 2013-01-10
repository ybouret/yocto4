#include "yocto/utest/run.hpp"
#include "./main.hpp"

#include "yocto/spade/array1d.hpp"
#include "yocto/spade/data-block.hpp"

YOCTO_UNIT_TEST_IMPL(array1D)
{
    for(size_t iter=0; iter<16;++iter)
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
            A1[i] = float(i);
        }
        
        linear *h = 0;
        void   *p = array1D<unsigned>::ctor(L1,&h);
        array1D<unsigned>::dtor(p);
    }
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/array2d.hpp"

YOCTO_UNIT_TEST_IMPL(array2D)
{
    for(size_t iter=0; iter<16;++iter)
    {
        const layout2D L2( RandCoord2D(), RandCoord2D());
        array2D<int>   A2( L2 );
        std::cerr << "A2.items=" << A2.items  << std::endl;
        std::cerr << "A2.bytes=" << A2.bytes  << std::endl;
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
        
        linear *h = 0;
        void   *p = array2D<unsigned>::ctor(L2,&h);
        array1D<unsigned>::dtor(p);

    }
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/array3d.hpp"

YOCTO_UNIT_TEST_IMPL(array3D)
{
    for(size_t iter=0; iter<16;++iter)
    {
        const layout3D   L3( RandCoord3D(), RandCoord3D());
        array3D<double>  A3( L3 );
        std::cerr << "A3.items=" << A3.items << std::endl;
        std::cerr << "A3.bytes=" << A3.bytes << std::endl;
        std::cerr << "A3: " << A3.as_layout() << std::endl;
        data_block B3(A3);
        
        A3.ldz();
        for( unit_t k=A3.lower.z;k<=A3.upper.z;++k)
        {
            for( unit_t j=A3.lower.y;j<=A3.upper.y;++j)
            {
                for( unit_t i=A3.lower.x; i <= A3.upper.x; ++i )
                {
                    A3[k][j][i] = double(i*j*k);
                }
            }
        }
        
        linear *h = 0;
        void   *p = array3D<unsigned>::ctor(L3,&h);
        array1D<unsigned>::dtor(p);

    }
}
YOCTO_UNIT_TEST_DONE()

