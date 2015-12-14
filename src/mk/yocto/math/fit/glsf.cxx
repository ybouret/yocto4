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
