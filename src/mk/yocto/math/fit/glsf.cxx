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
        void GLS<real_t>:: Sample:: link( size_t iGlobal, size_t iLocal )
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
                const real_t d  = Y[i] - F(Xi,u);
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
        real_t GLS<real_t>:: Samples:: probe(const real_t u)
        {
            assert(hook);
            assert(pvar);
            tao::setprobe(atry,*pvar,u,step);
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
        drvs(),
        scale(1e-4),
        p10_min( -GLS<real_t>::GET_P10_MAX() ),
        p10_max( -p10_min               ),
        scan( this, & Samples::probe ),
        hook(0),
        pvar(0)
        {
        }

        template <>
        GLS<real_t>:: Samples:: ~Samples() throw() {}

        template <>
        GLS<real_t>:: Sample & GLS<real_t>:: Samples:: append( const Array &X, const Array &Y, Array &Z)
        {
            typename Sample::Pointer p( new Sample(X,Y,Z) );
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

#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

namespace yocto
{
    namespace math
    {
        template <>
        bool GLS<real_t>:: Samples:: fit_with(Function          &F,
                                              Array             &aorg,
                                              const array<bool> &used)
        {
            assert(aorg.size()==M);
            assert(used.size()==M);

            //__________________________________________________________________
            //
            //
            // initialize
            //
            //__________________________________________________________________
            hook        = &F;
            pvar        = &aorg;
            int    p10  = -4;
            real_t lam  = -1;
            real_t Horg = computeD2(F,aorg,used);
            real_t Hnew = Horg;

            Vector aerr(M);
            tao::ld(aerr,0);

            size_t count = 0;
            //__________________________________________________________________
            //
            //
            // main loop
            //
            //__________________________________________________________________
        CYCLE:
            ++count;
            //__________________________________________________________________
            //
            // adjust p10, then curvature is computed@aorg
            //__________________________________________________________________
            lam = find_acceptable_lambda(p10);
            if(lam<0)
            {
                return false; //! no scaling...
            }


            std::cerr << "beta=" << beta << std::endl;
            if(tao::norm_sq(beta)<=0)
            {
                // level 1 success
                goto EXTREMUM;
            }

            //__________________________________________________________________
            //
            // compute step
            //__________________________________________________________________
            LU<real_t>::solve(step,cinv,beta);
            std::cerr << "lam  = " << lam  << std::endl;
            std::cerr << "step = " << step << std::endl;
            if(tao::norm_sq(step)<=0)
            {
                // level 2 success
                goto EXTREMUM;
            }

            //__________________________________________________________________
            //
            // |beta|>0, |step|>0, try full Newton's step
            //__________________________________________________________________
            Hnew = scan(1);
            if(Hnew<Horg)
            {
                std::cerr << "SUCCESS" << std::endl;
                //______________________________________________________________
                //
                // we take the step
                //______________________________________________________________
                tao::set(aorg,atry);
            }
            else
            {
                std::cerr << "FAILURE" << std::endl;
                //______________________________________________________________
                //
                // find a guess descent: 'towards' beta
                //______________________________________________________________
                for(size_t i=M;i>0;--i)
                {
                    if( (beta[i]*step[i]) < 0 )
                    {
                        step[i] = 0;
                    }
                }
                if(tao::norm_sq(step)<=0)
                {
                    // numeric noise...
                    goto EXTREMUM;
                }

                triplet<real_t> XX = { 0, 1 , 1 };
                triplet<real_t> HH = { Horg, scan(1), 0 };
                HH.c = HH.b;
                bracket<real_t>::expand(scan,XX,HH);
                minimize<real_t>(scan, XX, HH, 0);
                tao::setprobe(aorg, aorg, XX.b, step);
                if(++p10>p10_max)
                {
                    std::cerr << "Max decreased reached!" << std::endl;
                    goto EXTREMUM;
                }
            }

            //__________________________________________________________________
            //
            //
            // prepare next cycle
            //
            //__________________________________________________________________

            Hnew = computeD2(F,aorg,used);
            std::cerr << "H: " << Horg << " -> " << Hnew << std::endl;
            if(Fabs(Hnew-Horg)<= numeric<real_t>::ftol * Horg)
            {
                // numerical extremum
                goto EXTREMUM;
            }
            Horg = Hnew;
            goto CYCLE;
            
        EXTREMUM:
            //__________________________________________________________________
            //
            //
            // Final evaluation
            //
            //__________________________________________________________________
            std::cerr << "count=" << count << std::endl;
            Horg = computeD2(F,aorg,used);
            std::cerr << "curv=" << curv << std::endl;
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
                    std::cerr << "too many parameters" << std::endl;
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

                const real_t dS = Horg/dof;
                for(size_t i=M;i>0;--i)
                {
                    if( used[i] )
                    {
                        aerr[i] = Sqrt( Fabs(alpha[i][i]) * dS );
                    }
                }
                std::cerr << "aorg=" << aorg << std::endl;
                std::cerr << "aerr=" << aerr << std::endl;
                return true;
            }


        }
        
    }
    
}
