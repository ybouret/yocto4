#include "yocto/utest/run.hpp"
#include "yocto/sequence/multi-arrays.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(multi_arrays)
{
    
    multi_arrays<8, double, memory::global> M;
    
    for(size_t iter=1;iter<=100;++iter)
    {
        M.prepare(1+alea_lt(1000));
        std::cerr << "size=" << M.size << std::endl;
        
        //continue;
        
        for(size_t i=0;i<M.ARRAYS;++i)
        {
            array<double> &a = M[i];
            assert( a.size() == M.size );
            for(size_t j=M.size;j>0;--j)
            {
                a[j] = alea<double>();
            }
        }
    }
    
    
}
YOCTO_UNIT_TEST_DONE()

