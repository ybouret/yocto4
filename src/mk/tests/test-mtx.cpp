#include "yocto/container/matrix.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/string.hpp"
#include "yocto/code/rand.hpp"
#include "../../main/tests/support.hpp"
#include "yocto/mpl/rational.hpp"
#include "yocto/string/xnum.hpp"

using namespace yocto;
using namespace math;

#define __SHOW_SIZEOF(TYPE) std::cerr << "sizeof(matrix<" << #TYPE << ">)=" << sizeof(matrix<TYPE>) << std::endl

template <typename T>
static inline
void __test_access()
{
    {
        matrix<T> M0;
    }
    
    for(size_t iter=0;iter<1000;++iter)
    {
        matrix<T> M(1+alea_leq(20),1+alea_leq(20));
        for(size_t i=1;i<=M.rows;++i)
        {
            for(size_t j=1;j<=M.cols;++j)
            {
                M[i][j] = gen<T>::get();
            }
        }
        if(iter<=0)
        {
            std::cerr << "Generated " << M.rows << "x" << M.cols << std::endl;
            std::cerr << "M=" << M << std::endl;
        }
        matrix<T> P;
        P.swap_with(M);
        matrix<T> Q(P);
        
        matrix<T> R(P,YOCTO_MATRIX_TRANSPOSE);
        if(iter<=0)
            std::cerr << "R=" << R << std::endl;

        matrix<T> SQ(1+alea_leq(20));

        SQ.ldz();
    }
    
}

template <typename T>
static inline
void __test_mp()
{
    std::cerr << "Testing MP" << std::endl;
    {
        matrix<T> M0;
    }

    for(size_t iter=0;iter<1;++iter)
    {
        matrix<T> M(2+alea_leq(3),1+alea_leq(10));
        for(size_t i=1;i<=M.rows;++i)
        {
            for(size_t j=1;j<=M.cols;++j)
            {
                M[i][j] = int64_t(i+j);
            }
        }
        if(iter<=0)
        {
            std::cerr << "M=" << M << std::endl;
        }
        
        matrix<T> P(M);
        matrix<T> Q(M,YOCTO_MATRIX_TRANSPOSE);
        matrix<T> SQ(1+alea_leq(100));
    }
    
}


YOCTO_UNIT_TEST_IMPL(mtx)
{
    __SHOW_SIZEOF(int);
    __SHOW_SIZEOF(double);
    __SHOW_SIZEOF(string);
    
    __test_access<int>();
    __test_access<double>();
    __test_access<float>();
    __test_access< complex<double> >();
    __test_access< complex<float>  >();
    __test_access<string>();
    __test_mp<mpn>();
    __test_mp<mpz>();
    __test_mp<mpq>();
    
    
}
YOCTO_UNIT_TEST_DONE()
