#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/cholesky.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline
void perform()
{
    for(int iter=1;iter<=2;++iter)
    {
        for( size_t n=1; n <= 8; ++n )
        {
            matrix<T> M(n,n);
            M.ld1();
            for( size_t i=1; i <= n; ++i )
            {
                for( size_t j=1; j <= i; ++j )
                {
                    if( i != j )
                    {
                        M[i][j] = M[j][i] = T(0.5) - alea<T>();
                    }
                    else
                    {
                        M[i][i] += alea<T>();
                    }
                }
                //std::cerr << "M=" << M <<  std::endl;
            }
            vector<T> d(n,0);
            vector<T> b(n,0);
            vector<T> x(n,0);
            vector<T> r(n,0);
            const matrix<T> A = M;
            if(cholesky<T>::build( M, d ))
            {
                //std::cerr << "d=" << d << std::endl;
                
                for( size_t i=1; i <= n; ++i ) b[i] = T(0.5) - alea<T>();
                //std::cerr << "b=" << b << std::endl;
                cholesky<T>::solve( M,d,b,x);
                //std::cerr << "x=" << x << std::endl;
                algebra<T>::mul(r,A,x);
                algebra<T>::sub(r,b);
                std::cerr << "norm" << n << "=" << algebra<T>::norm2(r) << std::endl;
            }
            else {
                std::cerr << "// FAILURE" << std::endl;
            }
            
        }
    }
}

YOCTO_UNIT_TEST_IMPL(cholesky)
{
    std::cerr << "float" << std::endl;
	perform<float>();
    
    std::cerr << "double" << std::endl;
	perform<double>();
}
YOCTO_UNIT_TEST_DONE()
