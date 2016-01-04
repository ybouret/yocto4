#include "yocto/math/fcn/newt.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/math/core/svd.hpp"
#include "yocto/math/triplet.hpp"
#include "yocto/math/core/lu.hpp"
#include "yocto/code/ipower.hpp"
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
            tao::setprobe(xtry,xorg,lam,sigma);
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
        xtmp(),
        Ftmp(),
        M(2),
        rhs(2),
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
        bool  newt<real_t>:: solve( field &Field, array<real_t> &x )
        {

            static const real_t alpha     = 1e-4;
            static const real_t lambdaMin = 0.1;
            static const real_t lambdaMax = 0.5;

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
            xtmp.make(nvar);
            Ftmp.make(nvar);

            //__________________________________________________________________
            //
            // initialize: F and f_org must be computed
            // before entering CYCLE.
            //__________________________________________________________________
            Field(F,x);
            real_t phi0  = tao::norm_sq(F)/2;
            size_t dim_k = 0;
            real_t phi1  = phi0;

        CYCLE:
            std::cerr << "x="   << x      << std::endl;
            std::cerr << "F="   << F      << std::endl;
            std::cerr << "phi=" << phi0   << std::endl;
            //__________________________________________________________________
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
            // try full Newton's step: xtry = xorg + sigma
            //__________________________________________________________________
            real_t lam1 = 1;
            phi1 = scan(lam1);
            std::cerr << "phi1=" << phi1 << std::endl;
            const real_t slope = alpha * rho;
            if(phi1<=phi0-slope)
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

                //______________________________________________________________
                //
                // Initialize backtracking
                //______________________________________________________________
                const real_t beta0 = phi1 - (phi0-rho);
                std::cerr << "beta0=" << beta0 << std::endl;

                real_t lam2 = rho/(beta0+beta0);
                real_t phi2 = scan(lam2);
                std::cerr << "lam2=" << lam2 << std::endl;
                std::cerr << "phi2=" << phi2 << std::endl;

                for(size_t iter=1;
                    iter <=2
                    ;++iter)
                {
                    rhs[1] = phi1 - (phi0-lam1*rho);
                    rhs[2] = phi2 - (phi0-lam2*rho);

                    M[1][1] = ipower(lam1,2); M[1][2] = ipower(lam1,3);
                    M[2][1] = ipower(lam2,2); M[2][2] = ipower(lam2,3);

                    if( ! LU<real_t>::build(M) )
                    {
                        std::cerr << "numerical failure..." << std::endl;
                        return false;
                    }

                    LU<real_t>::solve(M,rhs);
                    const real_t beta = rhs[1];
                    const real_t gam3 = REAL(3.0)*rhs[2];
                    std::cerr << "beta=" << beta << std::endl;
                    std::cerr << "gam3=" << gam3 << std::endl;
                    const real_t dsc  = beta*beta+rho*gam3;
                    std::cerr << "dsc=" << dsc << std::endl;
                    const real_t alam = (Fabs(dsc)<=0) ? (lam1+lam2)/2 : (Sqrt(dsc)-beta)/gam3;
                    std::cerr << "alam=" << alam << std::endl;
                    const real_t aphi = scan(alam);

                    triplet<real_t> Lam = { alam, lam2, lam1 };
                    triplet<real_t> Phi = { aphi, phi2, phi1 };

                    std::cerr << "Lam=" << Lam << std::endl;
                    std::cerr << "Phi=" << Phi << std::endl;
                    Phi.co_sort(Lam);


                    std::cerr << "Lam=" << Lam << std::endl;
                    std::cerr << "Phi=" << Phi << std::endl;

                    lam1 = Lam.a; phi1 = Phi.a;
                    lam2 = Lam.b; phi2 = Phi.b;


                }

                return false;
            }

            //__________________________________________________________________
            //
            // F must be computed @xtry, with the f_new value
            //__________________________________________________________________
            phi0 = phi1;
            tao::set(x,xtry);
            goto CYCLE;

            return false;
        }




    }

}
