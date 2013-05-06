#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/diag.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"

#include <typeinfo>

using namespace yocto;
using namespace math;


template <typename T>
static inline
void test_diag()
{
    std::cerr << std::endl << "Testing for <" << typeid(T).name() << ">" << std::endl;
    
    for(size_t iter=1; iter <= 1; ++iter )
    {
        const size_t   n = 1 + alea_lt(10);
        matrix<T> a(n,n);
        
        for(size_t i=1; i<=n; ++i )
        {
            for(size_t j=1; j<=n;++j)
            {
                a[i][j] = (0.5-alea<double>());// + (i==j?a.rows:0);
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

YOCTO_UNIT_TEST_IMPL(eigv)
{
    matrix<double> A(3,3);
    A.ld1();
    A[1][1] = 1.1;
    A[2][1] = 0.01;
    vector<double> wr(3,0);
    wr[1] = A[1][1] + alea<double>();
    
    std::cerr << "A=" << A << std::endl;
    
    matrix<double> ev(1,3);
    diag<double>::eigv(ev, A, wr);
    
    
}
YOCTO_UNIT_TEST_DONE()


