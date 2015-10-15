#include "yocto/mpa/rational.hpp"
#include "yocto/math/core/tridiag.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/math/core/tao.hpp"

using namespace yocto;
using namespace math;

template <typename T>
static inline void test_td()
{
    for(size_t n=1;n<=8;++n)
    {
        std::cerr << std::endl;
        std::cerr << "size=" << n << std::endl;
        TriDiag<T>  p(n);
        cTriDiag<T> q(n);
        vector<T>   r(n);
        for(size_t i=1;i<=n;++i)
        {
            p.a[i] = T(-1);
            p.c[i] = T(1);
            p.b[i] = T(3);

            q.a[i] = p.a[i];
            q.b[i] = p.b[i];
            q.c[i] = p.c[i];

            r[i] =  T(i);

        }
        std::cerr << "p=" << p << std::endl;
        if(n>=2) std::cerr << "q=" << q << std::endl;
        std::cerr << "r=" << r << std::endl;

        vector<T> xp(n);
        vector<T> xq(n);
        vector<T> y(n);

        if(!p.solve(xp,r))
        {
            std::cerr << "TriDiag failure!" << std::endl;
        }
        else
        {
            std::cerr << "xp=" << xp << std::endl;
            p.apply(y,xp);
            std::cerr << "yp=" << y << std::endl;
            tao::sub(y,r);
            std::cerr << "dp=" << y << std::endl;
        }

        if(n>=2)
        {
            if(!q.solve(xq,r) )
            {
                std::cerr << "cTriDiag failure!" << std::endl;
            }
            else
            {
                std::cerr << "xq=" << xq << std::endl;
                q.apply(y,xq);
                std::cerr << "yq=" << y << std::endl;
                tao::sub(y,r);
                std::cerr << "dp=" << y << std::endl;
            }
        }
    }
}

YOCTO_UNIT_TEST_IMPL(tridiag)
{
    test_td<float>();
    test_td<double>();
    test_td< complex<float>  >();
    test_td< complex<double> >();
    test_td<mpq>();
    
}
YOCTO_UNIT_TEST_DONE()

