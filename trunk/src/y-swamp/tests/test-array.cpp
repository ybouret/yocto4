
#include "yocto/utest/run.hpp"

#include "yocto/swamp/array3d.hpp"

#include "yocto/code/rand.hpp"
#include "yocto/swamp/data-block.hpp"

using namespace yocto;
using namespace swamp;


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
    offsets_list offlist;
    
    for( size_t iter=0; iter < 8; ++iter )
    {
        std::cerr << "iter=" << iter << std::endl;
        {
            const layout1D L( rand1(), rand1() );
            array1D<float> A(L);
            data_block     mb(A);
            std::cerr << L << std::endl;
            A.ldz();
            
            for( unit_t i=A.lower; i <= A.upper; ++i )
            {
                A[i] = float(i);
            }
            
            if( A.lower < A.upper -1 )
            {
                const layout1D sub( A.lower+1, A.upper-1);
                offlist.free();
                A.load_offsets(sub,offlist);
                std::cerr << "#offsets=" << offlist.size() << std::endl;
                for( size_t i=1; i <= offlist.size(); ++i )
                {
                    std::cerr << offlist[i] << " ";
                }
                std::cerr << std::endl;
            }
            
        }  
        
        {
            const layout2D  L( rand2(), rand2() );
            array2D<double> A(L);
            data_block      mb(A);
            std::cerr << L << std::endl;
            A.ldz();
            
            for( unit_t j=A.lower.y; j <= A.upper.y; ++j )
            {
                for( unit_t i=A.lower.x; i <= A.upper.x; ++i )
                {
                    A[j][i] = double(i*j);
                }
            }
            
            offlist.free();
            
            coord2D mid = (A.lower+A.upper)/unit_t(2);
            const layout2D sub( mid - A.width/unit_t(4), mid + A.width/unit_t(4));
            std::cerr << "sub: " << sub << std::endl;
            A.load_offsets(sub, offlist);
            std::cerr << "#offsets=" << offlist.size() << std::endl;
            
            
        }
        
        {
            const layout3D  L( rand3(), rand3() );
            array3D<double> A(L);
            data_block      mb(A);
            std::cerr << L << std::endl;
            A.ldz();
            
            for( unit_t k=A.lower.z; k <= A.upper.z; ++k )
            {
                for( unit_t j=A.lower.y; j <= A.upper.y; ++j )
                {
                    for( unit_t i=A.lower.x; i <= A.upper.x; ++i )
                    {
                        A[k][j][i] = double(i*j*k);
                    }
                }
            }
            offlist.free();
            A.load_offsets(A, offlist);
            std::cerr << "#offsets=" << offlist.size() << std::endl;
        }
        
        
    }
}
YOCTO_UNIT_TEST_DONE()
