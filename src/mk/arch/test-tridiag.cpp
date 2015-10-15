#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/tridiag.hpp"
#include "yocto/code/rand.hpp"
#include "../../main/tests/support.hpp"
#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include <typeinfo>

using namespace yocto;
using namespace math;


namespace
{
    
    
    template <typename T>
    static void test_xtri()
    {
        std::cerr << "Testing with <" << typeid(T).name() << ">" << std::endl;
        for(size_t iter=1; iter <= 20; ++iter )
        {
            const size_t n = 1 + alea_leq(128);
            tridiag<T>   M(n);
            ctridiag<T>  cM(n);
            vector<T>    r(n,numeric<T>::zero);
            vector<T>    u(n,numeric<T>::zero);
            vector<T>    v(n,numeric<T>::zero);
            
            for(size_t i=1; i <= n; ++i)
            {
                cM.a[i] = M.a[i] = gen<T>::get();
                cM.b[i] = M.b[i] = gen<T>::get();
                cM.c[i] = M.c[i] = gen<T>::get();
                r[i]   = gen<T>::get();
            }
            
            matrix<T> A(n,n);
            matrix<T> B(n,n);
            
            //------------------------------------------------------------------
            //-- simple tridiag
            //------------------------------------------------------------------
            if( !M.__solve(u,r) )
            {
                std::cerr << "Singular matrix" << std::endl;
                continue;
            }
            M.apply(v,u);
            double sum = 0;
            for( size_t i=1; i <=n; ++i)
            {
                sum += Fabs(r[i] - v[i]);
            }
            sum /= n;
            std::cerr << "TDIFF" << n << "  = " << sum << std::endl;
            
            A.ld1();
            if(!M.solve(A))
            {
                std::cerr << "Singular Matrix" << std::endl;
                continue;
            }
            M.apply(B,A);
            for(size_t i=1; i<=n;++i)
            {
                B[i][i] -= numeric<T>::one;
            }
            sum = 0;
            for(size_t i=1; i <=n; ++i )
            {
                for(size_t j=1;j<=n;++j)
                {
                    sum += Fabs(B[i][j]);
                }
            }
            sum /= (n*n);
            std::cerr << "TMDIF" << n << "  = " << sum << std::endl;
            
            //------------------------------------------------------------------
            //-- cyclic
            //------------------------------------------------------------------
            if(n>=2)
            {
                if( !cM.__solve(u,r) )
                {
                    std::cerr << "Singular cyclic matrix" << std::endl;
                    continue;
                }
                cM.apply(v,u);
                sum = 0;
                for( size_t i=1; i<=n; ++i)
                {
                    sum += Fabs(r[i] - v[i]);
                }
                sum /= n;
                std::cerr << "CDIFF" << n << "  = " << sum << std::endl;
                
                A.ld1();
                if(!cM.solve(A))
                {
                    std::cerr << "Singular Matrix" << std::endl;
                    continue;
                }
                cM.apply(B,A);
                for(size_t i=1; i<=n;++i)
                {
                    B[i][i] -= numeric<T>::one;
                }
                sum = 0;
                for(size_t i=1; i <=n; ++i )
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        sum += Fabs(B[i][j]);
                    }
                }
                sum /= (n*n);
                std::cerr << "CMDIF" << n << "  = " << sum << std::endl;
            }
            
        }
        
    }
    
}

YOCTO_UNIT_TEST_IMPL(tridiag)
{
    test_xtri<float>();
    test_xtri<double>();
    test_xtri< complex<float> >();
    test_xtri< complex<double> >();
    
}
YOCTO_UNIT_TEST_DONE()
