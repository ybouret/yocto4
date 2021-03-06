#include "yocto/math/core/symdiag.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/core/determinant.hpp"
#include "yocto/math/core/cholesky.hpp"

using namespace yocto;
using namespace math;

#define AMPLITUDE 10

template <typename T>
static inline
void do_symdiag()
{
    for(size_t n=1;n<=16;++n)
    {
        matrix<T> a(n);
        matrix<T> Q(n,n);
        vector<T> d(n,0);
        for(size_t iter=0;iter<1;++iter)
        {
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=i;j<=n;++j)
                {
                    a[i][j] = a[j][i] = -T(AMPLITUDE)/2 + alea<T>() * AMPLITUDE;
                }
            }
            //std::cerr << "a=" << a << std::endl;
            matrix<T> a0(a,YOCTO_MATRIX_TIGHTEN);
            if(!symdiag<T>::build(a,d,Q))
            {
                std::cerr << "symdiag failure!!" << std::endl;
                continue;
            }
            //std::cerr << "d=diag(" << d << ")" << std::endl;
            //std::cerr << "Q=" << Q << std::endl;
            symdiag<T>::eigsrt(d,Q);
            //std::cerr << "ds=diag(" << d << ")" << std::endl;
            //std::cerr << "Qs=" << Q << std::endl;
            symdiag<T>::eigsrtA(d,Q);
            //std::cerr << "dA=diag(" << d << ")" << std::endl;
            //std::cerr << "QA=" << Q << std::endl;
            const size_t nker = symdiag<T>::eiginv(d);
            std::cerr << "nker=" << nker << std::endl;

        }
    }
}

static inline
void do_symker()
{
    for(size_t n=1;n<=6;++n)
    {
        std::cerr << std::endl;
        matrix<double> G(n);
        matrix<double> Q(n);
        vector<double> d(n,0);
        vector<double> x(n,0);
        vector<double> b(n,0);
        vector<double> u(n,0);

        for(size_t iter=0;iter<2;++iter)
        {
            const size_t   m = n + alea_leq(n);
            matrix<unit_t> J(n,m);

            const unit_t nm = m;
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=m;++j)
                {
                    J[i][j] = -nm + unit_t(alea_leq(2*nm));
                }
            }

            for(size_t ncpy=alea_leq(1);ncpy>0;--ncpy)
            {
                const int r1 = 1+alea_lt(n);
                const int r2 = 1+alea_lt(n);
                if(r1!=r2)
                {
                    std::cerr << "copy row " << r1 << " to " << r2 << std::endl;
                    for(size_t j=1;j<=m;++j)
                    {
                        J[r2][j] = J[r1][j];
                    }
                }
            }

            //std::cerr << "J=" << J << std::endl;

            tao::row_gram(G,J);
            const double idet = floor(determinant(G)+0.5);
            std::cerr << "G=" << G << std::endl;
            if(!symdiag<double>::build(G,d,Q))
            {
                std::cerr << "symker failure!" << std::endl;
                continue;
            }
            symdiag<double>::eigsrtA(d,Q);
            if(Fabs(idet)<=0)
            {
                std::cerr << "// Found 0 determinant!!!" << std::endl;
                //d[n] = 0;
                std::cerr << "d=diag(" << d << ")" << std::endl;
                std::cerr << "Q=" << Q << std::endl;

            }
            else
            {
                matrix<double> L(G,YOCTO_MATRIX_ENLARGE);
                if( !cholesky<double>::build(L) )
                {
                    std::cerr << "Can't build Cholesky" << std::endl;
                }
                else
                {
                    std::cerr << "L=" << L << std::endl;
                    for(size_t i=1;i<=n;++i) b[i] = -nm + unit_t(alea_leq(2*nm));
                    cholesky<double>::solve(x,L,b);
                    tao::mul(u,G,x);
                    std::cerr << "b=" << b << std::endl;
                    std::cerr << "x=" << x << std::endl;
                    std::cerr << "u=" << u << std::endl;
                    tao::sub(u, b);
                    std::cerr << "dif=" << tao::norm(u) << std::endl;
                }

            }

            const size_t nker = symdiag<double>::eiginv(d);
            std::cerr << "NKER=" << nker << std::endl;
            std::cerr << "invD=diag(" << d << ")" << std::endl;
            matrix<double> iG(n,n);
            symdiag<double>::compute(iG, d, Q);
            std::cerr << "iG=" << iG << std::endl;
            matrix<double> P(n,n);
            tao::mmul(P,G,iG);
            std::cerr << "Prod=" << P << std::endl;
        }
    }

}


YOCTO_UNIT_TEST_IMPL(symker)
{
    do_symdiag<float>();
    do_symdiag<double>();
    
    do_symker();
}
YOCTO_UNIT_TEST_DONE()
