#include "yocto/utest/run.hpp"
#include "yocto/sequence/some-arrays.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(multi_arrays)
{
#if 0
    multi_arrays<8, double, memory::global> M;
    
    
    for(size_t iter=1;iter<=100;++iter)
    {
        M.allocate(1+alea_lt(1000));
        std::cerr << "size=" << M.size << std::endl;
        
        continue;
        
        for(size_t i=0;i<M.num_arrays();++i)
        {
            array<double> &a = M[i];
            assert( a.size() == M.size );
            for(size_t j=M.size;j>0;--j)
            {
                a[j] = alea<double>();
            }
        }
    }
#endif
    
}
YOCTO_UNIT_TEST_DONE()

