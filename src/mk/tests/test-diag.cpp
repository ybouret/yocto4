#include "yocto/utest/run.hpp"
#include "yocto/math/core/diag.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/string/conv.hpp"

#include <typeinfo>

using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(balance)
{
    if( argc > 1 )
    {
        const size_t n = strconv::to<size_t>(argv[1],"n");
        matrix<double> A(n,n);
        for(size_t i=1; i<=n; ++i )
        {
            for(size_t j=1; j<=n;++j)
            {
                A[i][j] = (0.5-alea<double>());// + (i==j?a.rows:0);
            }
        }
        std::cerr << "A=" << A << std::endl;
        matrix<double> B = A;
        diag<double>::HessenbergBalance(B);
        std::cerr << "B=" << B << std::endl;
        matrix<double> C = B;
        diag<double>::HessenbergReduce(C);
        std::cerr << "C=" << C << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()



template <typename T>
static inline
void test_diag()
{
    std::cerr << std::endl << "DIAG for <" << typeid(T).name() << ">" << std::endl;
    
    for(size_t iter=1; iter <= 16; ++iter )
    {
        const size_t   n = 1 + alea_lt(10);
        matrix<T> a(n,n);
        
        for(size_t i=1; i<=n; ++i )
        {
            for(size_t j=1; j<=n;++j)
            {
                a[i][j] = (T(0.5)-alea<T>());// + (i==j?a.rows:0);
            }
        }
        std::cerr << "a=" << a << std::endl;
        vector<T>   wr(n,0);
        vector<T>   wi(n,0);
        size_t nr = 0;
        
        matrix<T> aa(a);
        if( diag<T>::eig(aa, wr,wi,nr) )
        {
            std::cerr << "nr=" << nr << std::endl;
            std::cerr << "wr=" << wr << std::endl;
            std::cerr << "wi=" << wi << std::endl;
            std::cerr << std::endl;
        }
        else
            std::cerr << "Couldn't diag!" << std::endl;
    }
    
}

YOCTO_UNIT_TEST_IMPL(diag)
{
    test_diag<double>();
    test_diag<float>();
    
}
YOCTO_UNIT_TEST_DONE()

template <typename T>
static inline
void __perform_eigv( const matrix<T> &A )
{
    std::cerr << "A=" << A << std::endl;
    const size_t n = A.rows; assert(A.is_square());
    vector<T> wr(n,0);
    vector<T> wi(n,0);
    size_t    nr = 0;
    {
        matrix<T> AA(A);
        if( !diag<T>::eig(AA,wr,wi,nr) )
        {
            std::cerr << "Can't get eigenvalues" << std::endl;
            return;
        }
    }
    
    std::cerr << "nr=" << nr << std::endl;
    if(nr>0)
    {
        vector<T> lam(nr,0);
        for(size_t i=1; i <= nr; ++i) lam[i] = wr[i];
        std::cerr << "lam=" << lam << std::endl;
        matrix<T> ev(nr,n);
        diag<T>::eigv(ev,A,lam);
        std::cerr << "ev=" << ev << "'" << std::endl;
    }
}

template <typename T>
static inline
void __test_eigv()
{
    std::cerr << std::endl << "EIGV for <" << typeid(T).name() << ">" << std::endl;
    for(size_t iter=1; iter <= 3; ++iter )
    {
        const size_t   n = 1 + alea_lt(10);
        matrix<T> A(n,n);
        
        for(size_t i=1; i<=n; ++i )
        {
            for(size_t j=1; j<=n;++j)
            {
                A[i][j] = (T(0.5)-alea<T>());// + (i==j?a.rows:0);
            }
        }
        __perform_eigv(A);
    }
}


YOCTO_UNIT_TEST_IMPL(eigv)
{
    matrix<double> A(3,3);
    A.ld1();
    A[1][1] = 1 + alea<double>();
    A[2][1] = 0.5-alea<double>();
    A[3][1] = 0.5-alea<double>();
    __perform_eigv(A);
    
    __test_eigv<float>();
    
}
YOCTO_UNIT_TEST_DONE()


