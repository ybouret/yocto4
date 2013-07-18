#include "yocto/chemical/initializer.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/algebra.hpp"

namespace yocto
{
    namespace chemical
    {
        using namespace math;
        
        typedef algebra<double> mkl;
        
        
        static inline
        void __project(vector_t       &X,
                       const matrix_t &P,
                       const matrix_t &P2,
                       const lu_t     &L2,
                       vector_t       &Y,
                       const vector_t &Lam,
                       vector_t       &Mu ) throw()
        {
            
            //==================================================================
            // initialize first projection
            //==================================================================
            mkl::set(Mu, Lam);
            mkl::mulsub(Mu, P, X);
            L2.solve(P2, Mu);
            mkl::mul_trn(Y, P, Mu);
            std::cerr << "Y=" << Y << std::endl;
            mkl::add(X, Y);
            double Ymin = mkl::norm2(Y);
            
            //==================================================================
            // go on until norm decreases
            //==================================================================
            while(true)
            {
                mkl::set(Mu, Lam);
                mkl::mulsub(Mu, P, X);
                L2.solve(P2, Mu);
                mkl::mul_trn(Y, P, Mu);
                std::cerr << "Y=" << Y << std::endl;
                mkl::add(X, Y);
                const double Ytmp = mkl::norm2(Y);
                if(Ytmp>=Ymin)
                    return;
                Ymin = Ytmp;
            }
        }
        
#define __PROJ(X) __project(X,P,P2,L2,Y,Lam,Mu)
        
        
        void initializer:: operator()( equilibria &cs, collection &lib, const double t )
        {
            //==================================================================
            //
            // initialize the equilibria
            //
            //==================================================================
            cs.build_from(lib);
            const size_t N = cs.size();
            const size_t M = lib.size();
            const size_t Nc = size();
            if( N+Nc != M )
                throw exception("initialier: #species=%u != (#equilibria=%u+#constraints=%u)", unsigned(M), unsigned(N), unsigned(Nc) );
            
            //==================================================================
            //
            // build the algebraic constraints
            //
            //==================================================================
            matrix_t P(Nc,M);
            vector_t Lam(Nc,0);
            for(size_t i=1; i <= Nc; ++i )
            {
                const constraint &Q = * (*this)[i];
                Lam[i] = Q.value;
                for( constraint_weights::const_iterator weight=Q.begin(); weight != Q.end(); ++weight )
                {
                    const species::ptr &sp = lib[weight->key];
                    P[i][sp->indx] = *weight;
                }
            }
            std::cerr << "P=" << P << std::endl;
            std::cerr << "Lam=" << Lam << std::endl;
            
            //==================================================================
            //
            // build the orthogonal matrix by SVD
            //
            //==================================================================
            matrix_t Q(N,M);
            {
                matrix_t F(M,M);
                for(size_t i=1;i<=Nc;++i)
                {
                    for(size_t j=1;j<=M;++j)
                        F[j][i] = P[i][j];
                }
                
                matrix_t __V(M,M);
                vector_t __W(M,0);
                if( !math::svd<double>::build(F, __W, __V) )
                    throw exception("singular initializer for SVD");
                for(size_t i=1;i<=N;++i)
                {
                    for(size_t j=1;j<=M;++j)
                        Q[i][j] = F[j][i+Nc];
                }
            }
            std::cerr << "Q=" << Q << std::endl;
            
            //==================================================================
            //
            // build the partial Moore-Penrose Pseudo Inverse
            //
            //==================================================================
            lu_t     L2(Nc);
            matrix_t P2(Nc,Nc);
            mkl::mul_rtrn(P2,P,P);
            if( !L2.build(P2) )
                throw exception("singular initializer for pseudo-inverse");
            
            //==================================================================
            //
            // Initialize the algorithm
            //
            //==================================================================
            vector_t &Y = cs.dC;
            vector_t &X = cs.C;
            vector_t  Mu(Nc,0);
            matrix_t &W   = cs.W;
            matrix_t &Phi = cs.Phi;
            vector_t &Gam = cs.Gamma;
            vector_t &xi  = cs.xi;
            vector_t  dX(M,0);
            const double ftol = cs.ftol;
            
            //==================================================================
            //
            // Algorithm
            //
            //==================================================================
            
            //------------------------------------------------------------------
            //
            // choose a starting point
            //
            //------------------------------------------------------------------
            for(size_t i=1;i<=M;++i)
                X[i] = 0;
            std::cerr << "X0=" << X << std::endl;
            __PROJ(X);
            std::cerr << "X0p=" << X << std::endl;
            
        NEWTON_STEP:
            //------------------------------------------------------------------
            //
            // compute the modified Jacobian
            //
            //------------------------------------------------------------------
            cs.compute_Gamma_and_Phi(t,false);
            mkl::mul_rtrn(W,Phi,Q);
            if( !cs.LU.build(W) )
            {
                std::cerr << "singular composition" << std::endl;
                return;
            }
            
            //------------------------------------------------------------------
            //
            // compute the chemical extent
            //
            //------------------------------------------------------------------
            mkl::neg(xi,Gam);
            cs.LU.solve(W,xi);
            
            //------------------------------------------------------------------
            //
            // compute Y=the increase in X
            //
            //------------------------------------------------------------------
            mkl::mul_trn(Y, Q, xi);
            
            //------------------------------------------------------------------
            //
            // compute the CORRECTED new X and the final differences
            //
            //------------------------------------------------------------------
            mkl::set(dX,X);
            mkl::add(X, Y);
            __PROJ(X);
            std::cerr << "X1=" << X << std::endl;
            mkl::sub(dX,X);
            
            //------------------------------------------------------------------
            //
            // check convergence
            //
            //------------------------------------------------------------------
            for(size_t i=M;i>0;--i)
            {
                if( Fabs(dX[i] ) > Fabs( ftol * X[i] ))
                {
                    goto NEWTON_STEP;
                }
            }
            
            //------------------------------------------------------------------
            //
            // success: check error
            //
            //------------------------------------------------------------------
            std::cerr << "ftol=" << ftol << std::endl;
            cs.compute_Gamma_and_Phi(t,false);
            mkl::mul_rtrn(W,Phi,Q);
            if( !cs.LU.build(W) )
            {
                std::cerr << "singular FINAL composition" << std::endl;
                return;
            }
            cs.LU.solve(W, Gam);
            mkl::mul_trn(dX, Q, Gam);
            std::cerr << "dX=" << dX << std::endl;
        }
        
    }
}
