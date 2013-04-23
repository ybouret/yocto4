#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/tridiag.hpp"
#include "yocto/code/rand.hpp"
#include "../../y-main/tests/support.hpp"
#include "yocto/math/types.hpp"
#include <typeinfo>

using namespace yocto;
using namespace math;


namespace
{
    
    template <typename T>
    void test_tri()
    {
        std::cerr << "Testing with <" << typeid(T).name() << ">" << std::endl;
        for(size_t iter=1; iter <= 10; ++iter )
        {
            const size_t n = 1 + alea_leq(128);
            
            tridiag<T> M(n);
            vector<T> r(n,numeric<T>::zero);
            vector<T> u(n,numeric<T>::zero);
            vector<T> v(n,numeric<T>::zero);
            
            for(size_t i=1; i <= n; ++i)
            {
                M.a(i) = gen<T>::get();
                M.b(i) = gen<T>::get();
                M.c(i) = gen<T>::get();
                
                r[i] = gen<T>::get();
            }
            
            M.solve(u,r);
            M.apply(v,u);
            double sum = 0;
            for( size_t i=1; i <=n; ++i)
            {
                sum += Fabs(r[i] - v[i]);
            }
            sum /= n;
            std::cerr << "DIFF" << n << "\t\t= " << sum << std::endl;
            
            matrix<T> A(n,n);
            matrix<T> B(n,n);
            B.ld1();
            M.solve(A,B);
            B.ldz();
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
            std::cerr << "MDIF" << n << "\t\t= " << sum << std::endl;
        }
    }
    
    
    
    
    template <typename T>
    static void test_xtri()
    {
        std::cerr << "Testing with <" << typeid(T).name() << ">" << std::endl;
        for(size_t iter=1; iter <= 10; ++iter )
        {
            const size_t n = 1 + alea_leq(128);
            xtridiag<T>  M(n);
            vector<T>    r(n,numeric<T>::zero);
            vector<T>    u(n,numeric<T>::zero);
            vector<T>    v(n,numeric<T>::zero);

            for(size_t i=1; i <= n; ++i)
            {
                M.a[i] = gen<T>::get();
                M.b[i] = gen<T>::get();
                M.c[i] = gen<T>::get();
                r[i]   = gen<T>::get();
            }
            
            //-- simple tridiag part
            //M.output( std::cerr << "M=" ) << std::endl;
            if( !M.solve(u,r) )
            {
                std::cerr << "Singular matrix" << std::endl;
                continue;
            }
            //std::cerr << "r=" << r << std::endl;
            //std::cerr << "u=" << u << std::endl;
            M.apply(v,u);
            double sum = 0;
            for( size_t i=1; i <=n; ++i)
            {
                sum += Fabs(r[i] - v[i]);
            }
            sum /= n;
            std::cerr << "DIFF" << n << "\t\t= " << sum << std::endl;
            
            matrix<T> A(n,n);
            matrix<T> B(n,n);
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
            std::cerr << "MDIF" << n << "\t\t= " << sum << std::endl;
            
        }
        
    }
    
}

YOCTO_UNIT_TEST_IMPL(tridiag)
{
    test_xtri<float>();
    test_xtri<double>();
    test_xtri< complex<float> >();
    test_xtri< complex<double> >();
    return 0;
    
    test_tri<float>();
    test_tri<double>();
    test_tri< complex<float> >();
    test_tri< complex<double> >();
    
}
YOCTO_UNIT_TEST_DONE()
