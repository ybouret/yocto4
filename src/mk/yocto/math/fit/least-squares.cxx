#include "yocto/math/ztype.hpp"
#include "yocto/math/fit/least-squares.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"

namespace yocto
{
    namespace math
    {
        namespace fit
        {
            typedef algebra<real_t> mkl;
            
            template <>
            least_squares<real_t>:: sample:: ~sample() throw()
            {
            }
            
            template <>
            least_squares<real_t>:: sample:: sample(const array<real_t> &aX, const array<real_t> &aY, array<real_t> &aZ ) throw() :
            X(aX),
            Y(aY),
            Z(aZ),
            N(0),
            beta(),
            alpha(),
            Gamma(),
            u(),
            dFdu(),
            D2(0)
            {
                
            }
            
            template <>
            void least_squares<real_t>:: sample::release() throw()
            {
                dFdu.  release();
                u.     release();
                Gamma. release();
                alpha. release();
                beta.  release();
                D2 = 0;
            }
            
            
            template <>
            void least_squares<real_t>:: sample:: prepare(size_t nvar, size_t npar)
            {
                assert(nvar>0);
                assert(npar>0);
                assert(X.size()==Y.size());
                assert(X.size()==Z.size());
                release();
                
                (size_t &)N = X.size();
                
                beta.  make(nvar,0);
                alpha. make(nvar,0);
                Gamma. make(npar,nvar);
                u.     make(npar,0);
                dFdu.  make(npar,0);
            }
            
            template <>
            real_t least_squares<real_t>:: sample:: compute_least_squares( function &F, const array<real_t> &a )
            {
                mkl::mul(u, Gamma, a);
                D2  = 0;
                for(size_t j=N;j>0;--j)
                {
                    Z[j] = F(X[j],u);
                    const real_t dd = Z[j] - Y[j];
                    D2 += dd;
                }
                return D2;
            }
            
            
        }
    }
}
