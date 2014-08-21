#include "yocto/math/kernel/tao.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"

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
    tao::mul(vt, m, vu);   std::cerr << vt << std::endl;
    tao::mul_add(vt,m,vu); std::cerr << vt << std::endl;
    tao::mul_sub(vt,m,vu); std::cerr << vt << std::endl;
    
    tao::mul_trn(vu, m, vt);     std::cerr << vu << std::endl;
    tao::mul_add_trn(vu, m, vt); std::cerr << vu << std::endl;
    tao::mul_sub_trn(vu, m, vt); std::cerr << vu << std::endl;

    
}


YOCTO_UNIT_TEST_IMPL(tao)
{
    test_tao<double,float,double>(10,12);
    test_tao<double,int,float>(8,10);
    

}
YOCTO_UNIT_TEST_DONE()
