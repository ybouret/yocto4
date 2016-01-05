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
            static const real_t lambdaTol = 1e-4; //!< for backtracking
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
            ++cycles; std::cerr << std::endl << "cycles=" << cycles << std::endl; //if(cycles>10) { return false; }
            std::cerr << "x="   << x      << std::endl;
            std::cerr << "F="   << F      << std::endl;

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
            //U.assign(J);
            if(!svd<real_t>::build(J,w,V))
            {
                // numeric failure to svd
                return false;
            }
            const size_t dim_k = svd<real_t>::truncate(w);
            //std::cerr << "w=" << w << std::endl;
            //std::cerr << "dim_k=" << dim_k << std::endl;

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
            std::cerr << "sigma=" << sigma << std::endl;
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
            //std::cerr << "rho=" << rho << std::endl;

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
            //std::cerr << "phi1=" << phi1 << std::endl;
            const real_t slope = alpha * rho;
            bool         is_ok = false;
            if(phi1<phi0-slope)
            {
                //______________________________________________________________
                //
                // accept full step: F is computed @xtry
                //______________________________________________________________
                is_ok = true;
            }
            else
            {
                std::cerr << "BACKTRACK" << std::endl;

                //______________________________________________________________
                //
                // Initialize backtracking
                //______________________________________________________________
                real_t lam1 = lambdaMax;
                phi1        = scan(lam1);

                real_t lam2 = lambdaMin;
                real_t phi2 = scan(lam2);

                while(Fabs(lam1-lam2) > lambdaTol)
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
                    real_t       alam = clamp<real_t>(0,ltmp,1);
                    real_t       aphi = scan(alam);

                    //__________________________________________________________
                    //
                    // sort to keep lam1, phi1 and lam2, phi2 as new guests
                    //__________________________________________________________
                    netsort<real_t>::co_level3<real_t>(phi1,phi2,aphi,lam1,lam2,alam);

                    std::cerr << "lam1=" << lam1 << "; phi1=" << phi1 << std::endl;
                    std::cerr << "lam2=" << lam2 << "; phi2=" << phi2 << std::endl;
                    std::cerr << "alam=" << alam << "; aphi=" << aphi << std::endl;
                    std::cerr << std::endl;
                }

                //______________________________________________________________
                //
                // recompute F@xtry=xorg+lam1*sigma
                //______________________________________________________________
                lam1  = clamp<real_t>(lambdaMin,lam1,lambdaMax);
                phi1  = scan(lam1);

                //______________________________________________________________
                //
                // acceptability criterium
                //______________________________________________________________
                is_ok = (phi1<phi0-lam1*slope);
            }

            //__________________________________________________________________
            //
            // F must be computed @xtry, with the phi1 value
            //__________________________________________________________________
            //std::cerr << "is_ok=" << is_ok << std::endl;
            phi0 = phi1;
            tao::set(x,xtry);
            goto CYCLE;
        }




    }

}
