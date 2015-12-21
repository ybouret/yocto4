#include "yocto/math/fcn/newt.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/math/core/svd.hpp"

namespace yocto
{

    namespace math
    {

        template <>
        real_t newt<real_t>:: eval( const real_t u )
        {
            array<real_t> &xorg = *pvar;
            const real_t   xsav =  xorg[ivar];
            xorg[ivar] = u;
            (*hook)(F,xorg);
            xorg[ivar] = xsav;
            return F[ifcn];
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
        scan(this, &newt<real_t>::eval),
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
                    J[ifcn][ivar] = drvs(scan,x[ivar],scaling);
                }
            }
        }

        template <>
        bool  newt<real_t>:: solve( field &Field, array<real_t> &x )
        {

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

            Field(F,x);
            real_t f_org = tao::norm_sq(F)/2;
            size_t dim_k = 0;

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

            //__________________________________________________________________
            //
            // Compute full Newton's step
            //__________________________________________________________________
            svd<real_t>::solve(U,w, V,F,sigma);
            std::cerr << "sigma=" << sigma << std::endl;

            

            return false;
            goto CYCLE;

            return false;
        }




    }

}
