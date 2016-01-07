#include "yocto/math/fcn/newt.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/math/core/svd.hpp"
#include "yocto/math/triplet.hpp"
#include "yocto/math/core/lu.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/math/triplet.hpp"
#include "yocto/sort/network.hpp"

namespace yocto
{

    namespace math
    {

        template <>
        real_t newt<real_t>:: __scan( const real_t lam )
        {
            assert(pvar);
            assert(hook);
            const array<real_t> &xorg = *pvar;
            tao::setprobe(xtry,xorg,lam,sigma);
            (*hook)(F,xtry);
            return REAL(0.5)*tao::norm_sq(F);
        }

        template <>
        newt<real_t>:: newt() :
        nvar(0),
        hook(0),
        pvar(0),
        J(),
        V(),
        arr(5),
        w(     arr.next_array() ),
        F(     arr.next_array() ),
        gradf( arr.next_array() ),
        sigma( arr.next_array() ),
        xtry(  arr.next_array() ),
        M(2),
        rhs(2),
        scan(this, &newt<real_t>::__scan)
        {
        }

        template <>
        newt<real_t>:: ~newt() throw()
        {
        }

        template <>
        bool  newt<real_t>:: solve( Field &func, Jacobian &fjac, array<real_t> &x )
        {

            static const real_t alpha     = 1e-4; //!< for descent rate
            static const real_t lambdaTol = 1e-3; //!< for backtracking
            static const real_t lambdaMin = 0.1;  //!< for backtracking
            static const real_t lambdaMax = 0.5;  //!< for backtracking

            //__________________________________________________________________
            //
            // setup
            //__________________________________________________________________
            nvar = x.size(); if(nvar<=0) return true;
            hook = &func;
            pvar = &x;

            //__________________________________________________________________
            //
            // allocate memory
            //__________________________________________________________________
            J.make(nvar,nvar);
            V.make(nvar,nvar);
            arr.allocate(nvar);


            //__________________________________________________________________
            //
            // initialize: F and phi0 must be computed
            // before entering CYCLE.
            //__________________________________________________________________
            func(F,x);
            real_t phi0   = REAL(0.5)*tao::norm_sq(F);
            real_t phi1   = phi0;
            size_t cycles = 0;

            goto CYCLE;

        SUCCESS:
            return true;

        CYCLE:
            ++cycles; //std::cerr << std::endl << "cycles=" << cycles << std::endl; //if(cycles>10) { return false; }
            //std::cerr << "x="   << x      << std::endl;
            //std::cerr << "F="   << F      << std::endl;
            //std::cerr << "phi0=" << phi0  << std::endl;

            if( phi0 <= 0 )
            {
                std::cerr << "[success] numeric zero" << std::endl;
                goto SUCCESS;
            }

            //__________________________________________________________________
            //
            // Jacobian and F @x, and grad(f)
            //__________________________________________________________________
            fjac(J,x);
            tao::mul_trn(gradf, J, F);

            if( tao::norm_sq(gradf) <= 0 )
            {
                std::cerr << "[success] numeric zero gradient" << std::endl;
                goto SUCCESS;
            }

            //__________________________________________________________________
            //
            // Compute SVD and truncate, keeping track of kernel size
            //__________________________________________________________________
            if(!svd<real_t>::build(J,w,V))
            {
                // numeric failure to svd
                return false;
            }
            const size_t dim_k = svd<real_t>::truncate(w);

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
            svd<real_t>::solve(J,w,V,F,sigma);
            tao::neg(sigma,sigma);
            if( tao::norm_sq(sigma) <= 0 )
            {
                std::cerr << "[success] zero Newton's step" << std::endl;
                goto SUCCESS;
            }

            //__________________________________________________________________
            //
            // Compute the descent rate
            //__________________________________________________________________
            const real_t rho = -tao::dot(sigma,gradf);
            if(rho<=0)
            {
                std::cerr << "[success] zero decrease rate" << std::endl;
                goto SUCCESS;
            }


            //__________________________________________________________________
            //
            // try full Newton's step: xtry = xorg + sigma
            //__________________________________________________________________
            phi1 = scan(1);
            const real_t slope = alpha * rho;
            if(phi1<phi0-slope)
            {
                //______________________________________________________________
                //
                // accept full step: F is computed @xtry
                //______________________________________________________________
            }
            else
            {
                std::cerr << "BACKTRACK" << std::endl;

                //______________________________________________________________
                //
                // Initialize backtracking, phi1=scan(lam1=1)
                //______________________________________________________________
                real_t lam1 = 1.0;
                
                real_t lam2 = lambdaMin;
                real_t phi2 = scan(lam2);
                
                //______________________________________________________________
                //
                // assuming lam1 is the most recent "minimum" position
                //______________________________________________________________

                if(phi2<=phi1)
                {
                    cswap(phi1, phi2);
                    cswap(lam1, lam2);
                }
                
                while(Fabs(lam1-lam2)>lambdaTol)
                {
                    
                    //__________________________________________________________
                    //
                    // compute the cubic approximation
                    //__________________________________________________________
                    rhs[1] = phi1 - (phi0-lam1*rho);
                    rhs[2] = phi2 - (phi0-lam2*rho);

                    M[1][1] = ipower(lam1,2); M[1][2] = ipower(lam1,3);
                    M[2][1] = ipower(lam2,2); M[2][2] = ipower(lam2,3);

                    if( !LU<real_t>::build(M) )
                    {
                        std::cerr << "numerical failure..." << std::endl;
                        return false;
                    }

                    LU<real_t>::solve(M,rhs);
                    const real_t beta = rhs[1];
                    const real_t gam3 = REAL(3.0)*rhs[2];
                    const real_t dsc  = beta*beta+rho*gam3;
                    const real_t ltmp = (Fabs(dsc)<=0) ? (lam1+lam2)/2 : (Sqrt(dsc)-beta)/gam3;
                    const real_t alam = clamp(lambdaMin,ltmp,lambdaMax);
                    const real_t aphi = scan(alam);

                    lam2 = lam1;
                    phi2 = phi1;
                    
                    lam1 = alam;
                    phi1 = aphi;

                    //std::cerr << "lam1=" << lam1 << "; phi1=" << phi1 << std::endl;
                    //std::cerr << "lam2=" << lam2 << "; phi2=" << phi2 << std::endl;

                }

                //______________________________________________________________
                //
                // recompute F@xtry=xorg+lam1*sigma
                //______________________________________________________________
                lam1  = lam2;
                phi1  = scan(lam1);
            }
            //__________________________________________________________________
            //
            // F must be computed @xtry, with the phi1 value:
            // copy new value and check convergence...
            //__________________________________________________________________
            real_t dx = 0;
            for(size_t i=nvar;i>0;--i)
            {
                const real_t x_new = xtry[i];
                const real_t x_old = x[i];
                const real_t tmp   = Fabs(x_old-x_new)/max_of<real_t>(Fabs(x_new),1);
                if(tmp>dx) dx=tmp;
                x[i] = x_new;
            }
            if(dx<=0)
            {
                std::cerr << "[success] stuck variables" << std::endl;
                goto SUCCESS;
            }

            phi0 = phi1;

            goto CYCLE;
        }




    }

}
