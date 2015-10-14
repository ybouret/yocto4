#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/memory/pooled.hpp"

#include "yocto/sequence/some-arrays.hpp"
#include "yocto/sequence/many-arrays.hpp"

#include "support.hpp"

using namespace yocto;

template <typename T>
static inline void process_arrays( arrays_manager<T> &M )
{
    for(size_t iter=1;iter<=10;++iter)
    {
        for(size_t i=0;i<M.num_arrays();++i)
        {
            M.allocate(1+alea_lt(1000));
            std::cerr << "size=" << M.size << std::endl;


            for(size_t i=0;i<M.num_arrays();++i)
            {
                array<T> &a = M[i];
                assert( a.size() == M.size );
                for(size_t j=M.size;j>0;--j)
                {
                    a[j] = gen<T>::get(); //T(double((i+1)*j)*alea<double>());
                }
            }

        }
    }
}


YOCTO_UNIT_TEST_IMPL(multi_arrays)
{

    some_arrays<8, double, memory::global> M1;
    process_arrays(M1);

    some_arrays<12,int, memory::pooled> M2;
    process_arrays(M2);

    many_arrays<float,memory::global> M3(5+alea_lt(10));
    process_arrays(M3);


    many_arrays<uint64_t,memory::pooled> M4(8+alea_lt(10));
    process_arrays(M4);

    many_arrays<string,memory::pooled> M5(3+alea_lt(10));
    process_arrays(M5);
}
YOCTO_UNIT_TEST_DONE()

