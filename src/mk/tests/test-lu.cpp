#include "yocto/utest/run.hpp"

#include "yocto/mpa/rational.hpp"
#include "yocto/math/core/lu.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/core/determinant.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline
void __test_lu()
{
    bool dneg = false;
    for(size_t n=1;n<=8;++n)
    {

        std::cerr << std::endl;
        for(size_t iter=0;iter<10;++iter)
        {
            matrix<T> M(n);
            M.ld1();
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=n;++j)
                {
                    if(i==j)
                    {
                        M[i][j] = int64_t((n*n)+i);
                    }
                    else
                    {
                        const int64_t x = -int64_t(n) + int64_t(alea_leq(2*n));
                        //std::cerr << "x=" << x << std::endl;
                        M[i][j] = x;
                    }
                }

            }
            std::cerr << "M=" << M << std::endl;
            matrix<T> A(M,YOCTO_MATRIX_TIGHTEN);

            const T dd = determinant(A);
            std::cerr << "det=" << dd << std::endl;

            if(!LU<T>::build(M,&dneg))
            {
                std::cerr << "Can't build LU" << std::endl;
                continue;
            }
            else
            {
                //std::cerr << "LU=" << M << std::endl;
            }

            T ddd = xnumeric<T>::one();
            for(size_t i=1;i<=n;++i)
            {
                ddd *= M[i][i];
            }
            if(dneg) ddd = -ddd;
            std::cerr << "ddd=" << ddd << std::endl;


            vector<T> b(n,as_capacity);
            vector<T> x(n,as_capacity);
            vector<T> c(n,as_capacity);
            vector<T> d(n,as_capacity);
            {
                b.free();
                x.free();
                c.free();
                d.free();

                for(size_t i=1;i<=n;++i)
                {
                    const int64_t x = -int64_t(n) + int64_t(alea_leq(2*n));
                    const T       y(x);
                    b.push_back(y);
                    const int64_t x0 = 0;
                    const T       y0(x0);
                    c.push_back(y0);
                }
                std::cerr << "b=" << b << std::endl;
                x = b;

                LU<T>::solve(M,x);
                std::cerr << "x=" << x << std::endl;
                tao::mul(c,A,x);
                std::cerr << "c=" << c << std::endl;
                d = c;
                tao::sub(d,b);
                //std::cerr << "d=" << d << std::endl;
                typename real_of<T>::type sum = 0;
                for(size_t i=n;i>0;--i)
                {
                    sum += Fabs(d[i]);
                }
                std::cerr << "\t\t|diff|=" << sum << std::endl;
            }
            matrix<T> I(n,n);
            I.ld1();
            LU<T>::solve(M,I);
            std::cerr << "I=" << I << std::endl;
        }
        
        
    }
}

YOCTO_UNIT_TEST_IMPL(lu)
{
    
    __test_lu<float>();
    __test_lu<double>();
    __test_lu< complex<float> >();
    __test_lu< complex<double> >();
    __test_lu<mpq>();
    
}
YOCTO_UNIT_TEST_DONE()

