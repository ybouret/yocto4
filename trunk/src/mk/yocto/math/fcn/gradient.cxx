#include "yocto/math/ztype.hpp"
#include "yocto/math/fcn/gradient.hpp"
#include "yocto/sequence/lw-array.hpp"


namespace yocto
{
    namespace math
    {
        
        template <>
        real_t gradient<real_t>:: eval(real_t u)
        {
            assert(pF);
            assert(px);
            assert(ix>0);
            assert(ix<=px->size());
            array<real_t> &xx = *px;
            const real_t   x0 = xx[ix];
            xx[ix] = u;
            const real_t ans = (*pF)(xx);
            xx[ix] = x0;
            return ans;
        }
        
        
#if defined(_MSC_VER)
#pragma warning ( disable : 4355 )
#endif
        template <>
        gradient<real_t>:: gradient() :
        derivative<real_t>(),
        pF(0),
        ix(0),
        px(0),
        gf(this, & gradient<real_t>::eval )
        {}
        
        template <>
        gradient<real_t>:: ~gradient() throw() {}
        
        
        
        
        template <>
        void gradient<real_t>:: compute1(numeric<real_t>::scalar_field &F,
                                         array<real_t>                 &x,
                                         const real_t                   Fx,
                                         array<real_t>                &dFdx,
                                         const array<real_t>          &dx)
        {
            const size_t n = x.size();
            assert(x.size()==dFdx.size());
            assert(x.size()==dx.size());
            
            
            for(size_t i=n;i>0;--i)
            {
                const real_t xmid = x[i];
                const real_t step = dx[i];
                x[i];
                const real_t Ft = F(x);
                dFdx[i] = (Ft-Fx)/step;
                x[i] = xmid;
            }
            
        }
        
        template <>
        void gradient<real_t>:: compute2(numeric<real_t>::scalar_field &F,
                                         array<real_t>                 &x,
                                         array<real_t>                 &dFdx,
                                         const array<real_t>           &dx)
        {
            const size_t n = x.size();
            assert(x.size()==dFdx.size());
            
            for(size_t i=n;i>0;--i)
            {
                const real_t xmid  = x[i];
                const real_t step = dx[i];
                x[i] = xmid+step;
                const real_t Fp = F(x);
                x[i] = xmid-step;
                const real_t Fm = F(x);
                x[i] = xmid;
                dFdx[i] = (Fp-Fm)/(step+step);
            }
            
        }
        
        template <>
        void gradient<real_t>:: __compute(array<real_t>       &x,
                                          array<real_t>       &dFdx,
                                          const array<real_t> &dx)
        {
            assert(x.size()    == dx.size());
            assert(dFdx.size() == x.size() );
            assert(0!=pF);
            const size_t n=x.size();
            
            px = &x;
            derivative<real_t> &drvs = *this;
            for(ix=n;ix>0;--ix)
            {
                dFdx[ix] = drvs(gf,x[ix],dx[ix]);
            }
        }
        
        
        template <>
        void gradient<real_t>:: compute(scalar_field        &F,
                                        array<real_t>       &x,
                                        array<real_t>       &dFdx,
                                        const array<real_t> &dx)
        {
            assert(x.size()    == dx.size());
            assert(dFdx.size() == x.size() );
            pF = &F;
            __compute(x, dFdx, dx);
        }
        
        
        
        
        
    }
}

