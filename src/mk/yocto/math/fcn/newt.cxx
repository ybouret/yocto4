#include "yocto/math/fcn/newt.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/math/core/svd.hpp"
#include "yocto/math/triplet.hpp"

namespace yocto
{

    namespace math
    {

        template <>
        real_t newt<real_t>:: __eval( const real_t u )
        {
            assert(pvar);
            assert(hook);
            assert(ivar>0);
            assert(ivar<=nvar);
            assert(ifcn>0);
            assert(ifcn<=nvar);

            array<real_t> &xorg = *pvar;
            const real_t   xsav =  xorg[ivar];
            xorg[ivar] = u;
            (*hook)(F,xorg);
            xorg[ivar] = xsav;
            return F[ifcn];
        }

        template <>
        real_t newt<real_t>:: __scan( const real_t lam )
        {
            assert(pvar);
            assert(hook);
            const array<real_t> &xorg = *pvar;
            tao::setprobe(xtry, xorg, lam, sigma);
            (*hook)(F,xtry);
            return tao::norm_sq(F)/2;
        }

        template <>
        newt<real_t>:: newt() :
        nvar(0),
        ivar(0),
        ifcn(0),
        hook(0),
        pvar(0),
        F(),
        J(),
        U(),
        V(),
        w(),
        gradf(),
        sigma(),
        xtry(),
        eval(this, &newt<real_t>::__eval),
        scan(this, &newt<real_t>::__scan),
        drvs(),
        scaling(1e-4)
        {
        }

        template <>
        newt<real_t>:: ~newt() throw()
        {
        }


        template <>
        void newt<real_t>:: computeJ()
        {
            array<real_t> &x = *pvar;
            for(ifcn=1;ifcn<=nvar;++ifcn)
            {
                for(ivar=1;ivar<=nvar;++ivar)
                {
                    J[ifcn][ivar] = drvs(eval,x[ivar],scaling);
                }
            }
        }

        
        template <>
        bool newt<real_t>:: backtack(const real_t rho, const real_t slope)
        {
            
            return true;
        }
        
        template <>
        bool  newt<real_t>:: solve( field &Field, array<real_t> &x )
        {

            static const real_t alpha     = 1e-4;
            static const real_t lambdaMin = 0.1;
            static const real_t lambdaMax = 0.5;
            static const real_t lambdaDel = lambdaMax-lambdaMin;

#if 0
            size_t  p = 0;
            real_t  s = lambdaDel;
            while(lambdaMin+s>lambdaMin)
            {
                s/=2;
                ++p;
            }
            std::cerr << "p=" << p << std::endl;
#endif

            //__________________________________________________________________
            //
            // setup
            //__________________________________________________________________
            nvar = x.size(); if(nvar<=0) return true;
            hook = &Field;
            pvar = &x;
            
            F.make(nvar);
            J.make(nvar,nvar);
            U.make(nvar,nvar);
            V.make(nvar,nvar);
            w.make(nvar);
            gradf.make(nvar);
            sigma.make(nvar);
            xtry.make(nvar);

            //__________________________________________________________________
            //
            // initialize: F and f_org must be computed
            //__________________________________________________________________
            Field(F,x);
            real_t f_org = tao::norm_sq(F)/2;
            size_t dim_k = 0;
            real_t f_new = f_org;

        CYCLE:
            std::cerr << "x=" << x     << std::endl;
            std::cerr << "F=" << F     << std::endl;
            std::cerr << "f=" << f_org << std::endl;
            //__________________________________________________________________
            //
            // at this point, F and f_org must be computed
            //
            // Jacobian and F @x, and grad(f)
            //__________________________________________________________________
            computeJ();
            std::cerr << "J=" << J << std::endl;
            tao::mul_trn(gradf, J, F);
            std::cerr << "gradf=" << gradf << std::endl;

            //__________________________________________________________________
            //
            // Compute SVD and truncate, keeping track of kernel size
            //__________________________________________________________________
            U.assign(J);
            if(!svd<real_t>::build(U,w,V))
            {
                // numeric failure to svd
                return false;
            }
            std::cerr << "w0=" << w << std::endl;
            dim_k = svd<real_t>::truncate(w);
            std::cerr << "dim_k=" << dim_k << std::endl;
            std::cerr << "w=" << w << std::endl;
            std::cerr << "U=" << U << std::endl;
            std::cerr << "V=" << V << std::endl;

            //__________________________________________________________________
            //
            // Do we have a singular point ?
            //__________________________________________________________________
            if(dim_k>=nvar)
            {
                // singular point
                return false;
            }

            //__________________________________________________________________
            //
            // Compute full Newton's step
            //__________________________________________________________________
            svd<real_t>::solve(U,w, V,F,sigma);
            tao::neg(sigma,sigma);
            std::cerr << "sigma=" << sigma << std::endl;

            //__________________________________________________________________
            //
            // Compute the descent rate
            //__________________________________________________________________
            const real_t rho = -tao::dot(sigma,gradf);
            std::cerr << "rho=" << rho << std::endl;

            if(rho<=0)
            {
                //singular point level 2
                return false;
            }


            //__________________________________________________________________
            //
            // try Newton's step
            //__________________________________________________________________
            f_new = scan(1.0);
            std::cerr << "f_new=" << f_new << std::endl;
            const real_t slope = alpha * rho;
            if(f_new<=f_org-slope)
            {
                //______________________________________________________________
                //
                // accept full step
                //______________________________________________________________
                std::cerr << "FULL" << std::endl;

            }
            else
            {
                std::cerr << "Need to backtrack" << std::endl;
                (void)backtack(rho,slope);
                return false;
            }

            //__________________________________________________________________
            //
            // F must be computed @xtry, with the f_new value
            //__________________________________________________________________
            f_org = f_new;
            tao::set(x,xtry);
            goto CYCLE;

            return false;
        }




    }

}
