#include "yocto/math/fit/glsf.hpp"
#include "yocto/math/ztype.hpp"


namespace yocto
{
    namespace math
    {

        template <>
        int GLS<real_t>::GET_P10_MAX() throw()
        {
            return -int( floor(Log10(numeric<real_t>::epsilon)) );
        }

        template <>
        real_t GLS<real_t>:: Sample:: Eval( const real_t U )
        {
            assert(ivar>0);
            assert(ivar<=L);
            assert(hook);
            const real_t save = u[ivar];
            u[ivar] = U;
            const real_t ans  = (*hook)(xval,u);
            u[ivar] = save;
            return ans;
        }


        template <>
        GLS<real_t>:: Sample:: Sample( const Array &XX, const Array &YY, Array &ZZ ) :
        X(XX),
        Y(YY),
        Z(ZZ),
        N(0),
        L(0),
        M(0),
        Gamma(),
        u(),
        dFdu(),
        dFda(),
        beta(),
        curv(),
        F1( this, & Sample::Eval ),
        ivar(0),
        xval(0),
        hook(0)
        {
        }

        template <>
        GLS<real_t>:: Sample:: ~Sample() throw()
        {
        }

        template <>
        void GLS<real_t>:: Sample:: link( const size_t iLocal, size_t iGlobal )
        {
            assert(iGlobal>0);
            assert(iGlobal<=Gamma.cols);
            assert(iLocal>0);
            assert(iLocal<=Gamma.rows);
            Gamma[iLocal][iGlobal] = 1;
        }

        template <>
        real_t GLS<real_t>:: Sample:: computeD2(Function    &F,
                                                const Array &a)
        {
            assert(X.size()==Y.size());
            assert(Y.size()==Z.size());
            size_t &count = (size_t &)N;
            count = 0;
            assert(a.size()==M);
            tao::mul(u,Gamma,a);
            real_t D2 = 0;
            for(size_t i=X.size();i>0;--i)
            {
                const real_t d = Y[i] - (Z[i]=F(X[i],u));
                D2 += d*d;
                ++count;
            }
            return D2;
        }

        template <>
        real_t GLS<real_t>:: Sample:: computeD2(Function           &F,
                                                const Array        &a,
                                                derivative<real_t> &drvs,
                                                const real_t        du)
        {
            assert(X.size()==Y.size());
            assert(Y.size()==Z.size());
            assert(a.size()==M);

            hook = &F;

            size_t &count = (size_t &)N;
            count = 0;
            tao::mul(u,Gamma,a);
            tao::ld(beta,0);
            curv.ldz();

            real_t D2 = 0;
            for(size_t i=X.size();i>0;--i)
            {
                const real_t Xi = X[i];

                //______________________________________________________
                //
                // compute gradient along u
                //______________________________________________________
                for(size_t j=L;j>0;--j)
                {
                    ivar    = j;
                    xval    = Xi;
                    dFdu[j] = drvs(F1,u[j],du);
                }

                //______________________________________________________
                //
                // deduce gradient along a
                //______________________________________________________
                tao::mul_trn(dFda, Gamma, dFdu);


                //______________________________________________________
                //
                // compute curent D2
                //______________________________________________________
                const real_t d  = Y[i] - (Z[i]=F(Xi,u));
                D2 += d*d;

                //______________________________________________________
                //
                // update local descent direction and curvature
                //______________________________________________________
                for(size_t j=M;j>0;--j)
                {
                    const real_t dFda_j = dFda[j];
                    beta[j] += d * dFda_j;
                    for(size_t k=M;k>=j;--k)
                    {
                        curv[j][k] += dFda_j * dFda[k];
                    }
                }

                ++count;
            }
            return D2;
        }

        template <>
        void GLS<real_t>:: Sample:: prepare( size_t global_nvar, size_t local_nvar )
        {
            assert(global_nvar>0);
            assert(local_nvar>0);
            assert(local_nvar<=global_nvar);
            (size_t &)L = local_nvar;
            (size_t &)M = global_nvar;
            Gamma.make(L,M);
            u.make(L);
            dFdu.make(L);
            dFda.make(M);
            beta.make(M);
            curv.make(M,M);

            Gamma.ldz();
        }

        template <>
        void GLS<real_t>:: Sample:: prepare( size_t global_nvar )
        {
            prepare(global_nvar,global_nvar);
            Gamma.ld1();
        }



    }
}



namespace yocto
{
    namespace math
    {
        template <>
        real_t GLS<real_t>:: Samples:: computeD2_(Function &F, const Array &a )
        {
            real_t D2 = 0;
            _Samples &self = *this;
            for(size_t i=self.size();i>0;--i)
            {
                D2 += self[i]->computeD2(F,a);
            }
            return D2;
        }


        template <>
        real_t GLS<real_t>:: Samples:: Eval( const real_t z )
        {
            assert(pvar);
            assert(hook);
            tao::setprobe(atry, *pvar, z, step);
            return computeD2_(*hook,atry);
        }

        template <>
        GLS<real_t>:: Samples:: Samples(size_t n) :
        _Samples(n,as_capacity),
        M(0),
        beta(),
        step(),
        atry(),
        curv(),
        cinv(),
        drvs(),
        scale(1e-4),
        p10_min( -GLS<real_t>::GET_P10_MAX() ),
        p10_max( -p10_min               ),
        cycle(0),
        pvar(NULL),
        hook(NULL),
        scan( this , &Samples::Eval )
        {
        }

        template <>
        GLS<real_t>:: Samples:: ~Samples() throw() {}

        template <>
        GLS<real_t>:: Sample & GLS<real_t>:: Samples:: append( const Array &X, const Array &Y, Array &Z)
        {
            Sample::Pointer p( new Sample(X,Y,Z) );
            this->push_back(p);
            return *p;
        }

        template <>
        void GLS<real_t>:: Samples:: setup(size_t nvar)
        {
            assert(nvar>0);
            (size_t &)M = nvar;
            beta.make(M);
            step.make(M);
            atry.make(M);
            curv.make(M,M);
            cinv.make(M);    // large memory model for LU
        }


        template <>
        void GLS<real_t>:: Samples:: prepare(size_t global_nvar, size_t local_nvar)
        {
            _Samples &self = *this;
            for(size_t i=self.size();i>0;--i)
            {
                self[i]->prepare(global_nvar,local_nvar);
            }
            setup(global_nvar);
        }

        template <>
        void GLS<real_t>:: Samples::  prepare(size_t global_nvar)
        {
            _Samples &self = *this;
            for(size_t i=self.size();i>0;--i)
            {
                self[i]->prepare(global_nvar);
            }
            setup(global_nvar);
        }

        template <>
        real_t GLS<real_t>:: Samples:: computeD2(Function          &F,
                                                 const Array       &aorg,
                                                 const array<bool> &used)

        {
            assert(M==aorg.size());
            assert(M==used.size());

            //__________________________________________________________
            //
            // initialize
            //__________________________________________________________
            tao::ld(beta,0);
            curv.ldz();
            real_t D2   = 0;

            //__________________________________________________________
            //
            // full computation
            //__________________________________________________________
            _Samples &self = *this;
            for(size_t i=self.size();i>0;--i)
            {
                Sample &S = *self[i];
                D2 += S.computeD2(F,aorg,drvs,scale);
                for(size_t j=M;j>0;--j)
                {
                    beta[j] += S.beta[j];
                    for(size_t k=M;k>=j;--k)
                    {
                        curv[j][k] += S.curv[j][k];
                    }
                }
            }

            //__________________________________________________________
            //
            // legalizing
            //__________________________________________________________
            for(size_t j=M;j>0;--j)
            {
                if(used[j])
                {
                    for(size_t r=1;r<=M;++r)
                    {
                        for(size_t c=1;c<r;++c)
                        {
                            curv[r][c] = curv[c][r];
                        }
                    }
                }
                else
                {
                    beta[j] = 0;
                    for(size_t i=M;i>0;--i)
                    {
                        curv[i][j] = curv[j][i] = 0;
                    }
                    curv[j][j] = 1;
                }
            }

            return D2;
        }


    }

}


#include "yocto/math/core/lu.hpp"
namespace yocto
{
    namespace math
    {

        template <>
        bool GLS<real_t>:: Samples:: compute_cinv(const real_t lambda)
        {
            assert(lambda>=0);
            const real_t fac = real_t(1)+lambda;
            cinv.assign(curv);
            for(size_t i=M;i>0;--i)
            {
                cinv[i][i] *= fac;
            }
            return LU<real_t>::build(cinv);
        }

    }

}

#include "yocto/code/ipower.hpp"
namespace yocto
{
    namespace math
    {

        template <>
        real_t GLS<real_t>:: Samples:: compute_lambda(const int p10)
        {
            if(p10<=p10_min)
            {
                return 0;
            }
            else
            {
                const int p = min_of<int>(p10,p10_max);
                if(p>=0)
                {
                    return ipower<real_t>(10,size_t(p));
                }
                else
                {
                    return ipower<real_t>(0.1,size_t(-p));
                }
            }
        }

        template <>
        real_t GLS<real_t>:: Samples:: find_acceptable_lambda(int &p10)
        {
            assert(p10>=p10_min);
            assert(p10<=p10_max);
            real_t lambda = -1;
            while( !compute_cinv( (lambda=compute_lambda(p10)) ) )
            {
                ++p10;
                if(p10>p10_max)
                {
                    return -1;
                }
            }
            return lambda;
        }

    }
}

namespace yocto
{
    namespace math
    {
        template <>
        bool GLS<real_t>:: Samples:: fit_with(Function          &F,
                                              Array             &aorg,
                                              const array<bool> &used,
                                              Array             &aerr,
                                              Callback          *cb)
        {
            assert(aorg.size()==M);
            assert(used.size()==M);
            assert(aerr.size()==M);

            //__________________________________________________________________
            //
            //
            // initialize
            //
            //__________________________________________________________________
            const real_t ftol = numeric<real_t>::ftol;
            int          p10  = p10_min;
            real_t       lam  = -1;
            real_t       Horg = computeD2(F,aorg,used);
            real_t       Hnew = Horg;
            bool         cvg  = false;

            tao::ld(aerr,0);
            cycle = 0;

            pvar = &aorg;
            hook = &F;

            //__________________________________________________________________
            //
            //
            // main loop
            //
            //__________________________________________________________________
            real_t step_sq = 0;
            real_t beta_sq = tao::norm_sq(beta);
        CYCLE:
            if(beta_sq<=0)
            {
                // level 1 success
                goto EXTREMUM;
            }
            ++cycle;
            if(cb && !(*cb)(*this,aorg) )
            {
                return false;
            }
            //__________________________________________________________________
            //
            // At this point, beta and curv are computed @aorg
            // adjust p10, then convpute cinv
            //__________________________________________________________________
            lam = find_acceptable_lambda(p10);
            if(lam<0)
            {
                return false; //! no possible scaling, singular matrix ?
            }

            
            //__________________________________________________________________
            //
            // compute step
            //__________________________________________________________________
            LU<real_t>::solve(step,cinv,beta);
            step_sq = tao::norm_sq(step);
            if(step_sq<=0)
            {
                // level 2 success
                goto EXTREMUM;
            }

            //__________________________________________________________________
            //
            // |beta|>0, |step|>0, try full Newton's step
            //__________________________________________________________________
            Hnew = Eval(1.0);
            if(Hnew<Horg)
            {
                //______________________________________________________________
                //
                // we take the step
                //______________________________________________________________
                tao::set(aorg,atry);
                p10 = max_of(p10-1,p10_min);
            }
            else
            {
                //______________________________________________________________
                //
                // keep same parameters, decrease step through lambda
                //______________________________________________________________
                //std::cerr << "bad..." << std::endl;
                if(++p10>p10_max)
                {
                    goto EXTREMUM;
                }

                
                goto CYCLE;
            }
            
            //__________________________________________________________________
            //
            // test numerical convergence on H
            //__________________________________________________________________
            if( Fabs(Hnew-Horg) <= ftol*Horg )
            {
                // numerical extremum
                goto EXTREMUM;
            }

            //__________________________________________________________________
            //
            // test numerical convergence on variables
            //__________________________________________________________________
            cvg = true;
            for(size_t i=M;i>0;--i)
            {
                if( Fabs(step[i]) > Fabs( numeric<real_t>::ftol * aorg[i] ) )
                {
                    cvg = false;
                    break;
                }
            }
            if(cvg)
            {
                goto EXTREMUM;
            }

            //__________________________________________________________________
            //
            //
            // prepare next cycle, aorg is changed, recompute beta and curv
            //
            //__________________________________________________________________
            Horg    = computeD2(F,aorg,used);
            beta_sq = tao::norm_sq(beta);
            goto CYCLE;

        EXTREMUM:
            //__________________________________________________________________
            //
            //
            // Final evaluation
            //
            //__________________________________________________________________
            Horg = computeD2(F,aorg,used);
            if( !compute_cinv(0) )
            {
                std::cerr << "invalid curvature at extremum!" << std::endl;
                return false;
            }
            else
            {
                //______________________________________________________________
                //
                // variables
                //______________________________________________________________
                size_t       nvar = M;
                for(size_t i=M;i>0;--i)
                {
                    if( !used[i] )
                    {
                        --nvar;
                    }
                }

                //______________________________________________________________
                //
                // data
                //______________________________________________________________
                _Samples    &self = *this;
                size_t       ndat = 0;
                for(size_t i=self.size();i>0;--i)
                {
                    ndat += self[i]->N;
                }
                if(ndat<nvar)
                {
                    return false;
                }

                //______________________________________________________________
                //
                // Degrees of freedom
                //______________________________________________________________
                const size_t  dof = 1 + (ndat-nvar);

                //______________________________________________________________
                //
                // compute the covariance matrix
                //______________________________________________________________
                matrix<real_t> &alpha = curv;
                alpha.ld1();
                LU<real_t>::solve(cinv,alpha);

                //______________________________________________________________
                //
                // compute the variances..
                //______________________________________________________________
                const real_t dS = Horg/dof;
                for(size_t i=M;i>0;--i)
                {
                    if( used[i] )
                    {
                        aerr[i] = Sqrt( Fabs(alpha[i][i]) * dS );
                    }
                }
                return true;
            }


        }

    }

}

#include <iomanip>

namespace yocto
{

    namespace math
    {
        template <>
        void GLS<real_t>::display( std::ostream &os, const Array &aorg, const Array &aerr)
        {
            assert(aerr.size()==aorg.size());
            const size_t n = aorg.size();
            for(size_t i=1;i<=n;++i)
            {
                const real_t AA = aorg[i];
                const real_t AE = aerr[i];
                os << "\tparam[" << std::setw(3) << i << "]= " << std::setw(16) << AA  << " +/-" << std::setw(16) << AE;
                if(Fabs(AA)>0)
                {
                    std::cerr << " (" << (REAL(100.0)*AE/Fabs(AA)) << "%)";
                }
                os << std::endl;
            }
            
            
        }
        
    }
    
}
