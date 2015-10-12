#include "yocto/math/core/symdiag.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/math/core/determinant.hpp"

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
        vector<T>       d(n,0);
        for(size_t iter=0;iter<1;++iter)
        {
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=n;++j)
                {
                    a[i][j] = a[j][i] = -T(AMPLITUDE)/2 + alea<double>() * AMPLITUDE;
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

            for(size_t ncpy=1;ncpy>0;--ncpy)
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
                d[n] = 0;
                std::cerr << "d=diag(" << d << ")" << std::endl;
                std::cerr << "Q=" << Q << std::endl;

            }
            else
            {
                std::cerr << "det(G)=" << idet << std::endl;
            }

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
