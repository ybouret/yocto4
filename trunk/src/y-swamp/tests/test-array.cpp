
#include "yocto/utest/run.hpp"

#include "yocto/swamp/array3d.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/swamp/data-block.hpp"

using namespace yocto;
using namespace swamp;

template <typename LAYOUT>
static inline void display( const LAYOUT &L )
{
    std::cerr << "DIM= " << LAYOUT::DIMENSIONS << std::endl;
    std::cerr << L.lower << " -> " << L.upper << " = " << L.width << " | #=" << L.items << std::endl;
}

static inline coord1D rand1()
{
    return 20 - 40 * alea<double>();
}

static inline coord2D rand2()
{
    return coord2D( rand1(), rand1() );
}

static inline coord3D rand3() {    return coord3D( rand1(), rand1(), rand1() ); }


YOCTO_UNIT_TEST_IMPL(array)
{
    offsets offlist;
    
    for( size_t iter=0; iter < 8; ++iter )
    {
        std::cerr << "iter=" << iter << std::endl;
        {
            const layout1D L( rand1(), rand1() );
            array1D<float> A(L);
            data_block     mb(A);
            display(A);
            A.ldz();
            
            for( unit_t i=A.lower; i <= A.upper; ++i )
            {
                A[i] = i;
            }
            
            //const layout1D sub( L.lower/2, L.upper/2 );
            offlist.release();
            A.load_offsets(L,offlist);
            
            std::cerr << "#offsets=" << offlist.size() << std::endl;
            
        }  
        
        {
            const layout2D  L( rand2(), rand2() );
            array2D<double> A(L);
            data_block      mb(A);
            display(A);
            A.ldz();
            
            for( unit_t j=A.lower.y; j <= A.upper.y; ++j )
            {
                for( unit_t i=A.lower.x; i <= A.upper.x; ++i )
                {
                    A[j][i] = i*j;
                }
            }
        }
        
        {
            const layout3D  L( rand3(), rand3() );
            array3D<double> A(L);
            data_block      mb(A);
            display(A);
            A.ldz();
            
            for( unit_t k=A.lower.z; k <= A.upper.z; ++k )
            {
                for( unit_t j=A.lower.y; j <= A.upper.y; ++j )
                {
                    for( unit_t i=A.lower.x; i <= A.upper.x; ++i )
                    {
                        A[k][j][i] = i*j*k;
                    }
                }
            }
        }
        
        
    }
}
YOCTO_UNIT_TEST_DONE()
