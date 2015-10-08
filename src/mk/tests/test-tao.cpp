#include "yocto/math/kernel/tao.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/kernel/matrix.hpp"

using namespace yocto;
using namespace math;

template <typename T, typename U, typename V>
static inline
void test_tao(const size_t N, const size_t M)
{
    std::cerr << std::endl;
    
    vector<T> vt(N,0);
    vector<U> vu(M,0);
    
    tao::ld(vt,0);
    tao::ld(vu,1);
    
    tao::set(vt, vu);       std::cerr << vt << std::endl;
    tao::mulset(vt, 2,vu);  std::cerr << vt << std::endl;
    tao::muladd(vt,-1,vu);  std::cerr << vt << std::endl;
    tao::mulsub(vt, 5,vu);  std::cerr << vt << std::endl;
    
    matrix<V> m(N,M);
    for(size_t i=N;i>0;--i)
    {
        for(size_t j=M;j>0;--j)
        {
            m[i][j] = V(int( 10.0 * (alea<double>() - 0.5) ));
        }
    }
    std::cerr << "m=" << m << std::endl;
    
    tao::mul(vt,m,vu);     std::cerr << vt << std::endl;
    tao::mul_add(vt,m,vu); std::cerr << vt << std::endl;
    tao::mul_sub(vt,m,vu); std::cerr << vt << std::endl;
    
    tao::mul_trn(vu, m, vt);     std::cerr << vu << std::endl;
    tao::mul_add_trn(vu, m, vt); std::cerr << vu << std::endl;
    tao::mul_sub_trn(vu, m, vt); std::cerr << vu << std::endl;
    
    for(size_t iter=1;iter<=10;++iter)
    {
        const size_t ns = 1 + alea_leq(N+M);
        matrix<V> b(N,ns);
        matrix<V> c(ns,M);
        tao::mmul(m,b,c);
        matrix<V> d(M,ns);
        tao::mmul_rtrn(m,b,d);
    }
    
    for(size_t iter=1;iter<=3;++iter)
    {
        const T fac( int( 16.0 * (alea<double>() - 0.5) ) );
        for(size_t i=N;i>0;--i)
        {
            vt[i] = T(0);
            if(alea<double>()>0.5) vt[i] = fac * T(int( 16.0 * (alea<double>() - 0.5) ));
        }
        std::cerr << "v0=" << vt << std::endl;
        (void)tao::simplify(vt);
        std::cerr << "v1=" << vt << std::endl;
    }
    
    for(size_t iter=1;iter<=3;++iter)
    {
        matrix<T> H(N,N);
        vector<T> X(N,0);
        for(size_t i=N;i>0;--i)
        {
            for(size_t j=N;j>=i;--j)
            {
                H[i][j] = H[j][i] = V(int( 10.0 * (alea<double>() - 0.5) ));
            }
            X[i] = T(10*(alea<double>()-(0.5)));
        }
        const T q = tao::quadratic(H, X);
        vector<T> tmp(N,0);
        tao::mul(tmp, H, X);
        const T d = tao::dot(tmp, X);
        std::cerr << "q=" << q << std::endl;
        std::cerr << "d=" << d << std::endl;
    }
}


YOCTO_UNIT_TEST_IMPL(tao)
{
    test_tao<double,float,double>(10,12);
    test_tao<double,int,float>(8,10);
    
    
}
YOCTO_UNIT_TEST_DONE()
