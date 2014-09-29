#include "yocto/math/ztype.hpp"
#include "yocto/math/fcn/gradient.hpp"
#include "yocto/sequence/lw-array.hpp"


namespace yocto
{
    namespace math
    {
        
        template <>
        gradient<real_t>:: gradient() throw() : var() {}
        
        template <>
        gradient<real_t>:: ~gradient() throw() {}
        
        template <>
        void gradient<real_t>:: compute(numeric<real_t>::scalar_field &F,
                                        const array<real_t>           &x,
                                        const real_t                   Fx,
                                        array<real_t>                &dFdx,
                                        const array<real_t>          &dx)
        {
            const size_t n = x.size();
            assert(x.size()==dFdx.size());
            assert(x.size()==dx.size());
            
            var.make(n,0);
            for(size_t i=n;i>0;--i)
            {
                var[i] = x[i];
            }
            
            for(size_t i=n;i>0;--i)
            {
                const real_t save = var[i];
                const real_t step = dx[i];
                var[i] += step;
                const real_t Ft = F(var);
                dFdx[i] = (Ft-Fx)/step;
                var[i] = save;
            }
            
        }
        
        template <>
        void gradient<real_t>:: compute(numeric<real_t>::scalar_field &F,
                                        const array<real_t>           &x,
                                        array<real_t>                 &dFdx,
                                        const array<real_t>           &dx)
        {
            const size_t n = x.size();
            assert(x.size()==dFdx.size());
            var.make(n,0);
            for(size_t i=n;i>0;--i)
            {
                var[i] = x[i];
            }
            
            for(size_t i=n;i>0;--i)
            {
                const real_t mid  = var[i];
                const real_t step = dx[i];
                var[i] = mid+step;
                const real_t Fp = F(var);
                var[i] = mid-step;
                const real_t Fm = F(var);
                var[i] = mid;
                dFdx[i] = (Fp-Fm)/(step+step);
            }
            
        }
        
        template <>
        void gradient<real_t>:: compute( numeric<real_t>::scalar_field &F,
                                        const array<real_t>            &x,
                                        const real_t                    Fx,
                                        array<real_t>                  &dFdx,
                                        const array<real_t>            &dx,
                                        matrix<real_t>                 &H)
        {
            const size_t n = x.size();
            assert(x.size() == dFdx.size());
            assert(x.size() ==   dx.size());
            assert(H.rows   ==   x.size());
            assert(H.cols   ==   x.size());
            lw_array<real_t> Fp( H.get_aux<real_t>(),  n );
            lw_array<real_t> Fm( H.get_aux2<real_t>(), n );

            var.make(n,0);
            for(size_t i=n;i>0;--i)
            {
                var[i] = x[i];
            }
            H.ldz();
            
            const real_t twoFx = Fx+Fx;
            
            for(size_t i=n;i>0;--i)
            {
                const real_t xmid = var[i];
                const real_t step = dx[i];
                var[i] = xmid+step;
                Fp[i]  = F(var);
                var[i] = xmid-step;
                Fm[i]  = F(var);
                var[i] = xmid;
                
                dFdx[i] = (Fp[i]-Fm[i])/(step+step);
                H[i][i] = (Fp[i] - twoFx + Fm[i])/(step*step);
            }
            
            
        }
        
        
        
    }
}

