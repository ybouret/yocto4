#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/math/kernel/svd.hpp"
#include "yocto/auto-clean.hpp"


namespace yocto
{
    namespace chemical
    {
        
        using namespace math;
        typedef algebra<double> mkl;
        typedef svd<double>     SVD;
        
        
        namespace
        {
            
            
            static inline bool has_maximal_rank( const matrix_t &A, lu_t &LU )
            {
                const size_t nr = A.rows;
                const size_t nc = A.cols;
                assert( LU.capacity() >= max_of(A.rows, A.cols));
                
                if(nr>nc)
                {
                    matrix_t A2(nc,nc);
                    mkl::mul_ltrn(A2, A, A);
                    return LU.build(A2);
                }
                else
                {
                    matrix_t A2(nr,nr);
                    mkl::mul_rtrn(A2, A, A);
                    return LU.build(A2);
                }
                
            }
            
        }
        
        
        void boot::loader::operator()(equilibria &cs, collection &lib, double t)
        {
            
            static const double zero = numeric<double>::zero;
            
            //__________________________________________________________________
            //
            //
            // Initializing and check dimensions
            //
            //__________________________________________________________________
            const size_t M  = lib.size();
            const size_t N  = cs.size();
            const size_t Nc = this->size();
            if( Nc+N != M )
                throw exception("#species=%u != (#equilibria=%u+#constraints=%u)", unsigned(M), unsigned(N), unsigned(Nc) );
            
            //__________________________________________________________________
            //
            //
            // Acquiring resources for equilibria
            //
            //__________________________________________________________________
            cs.build_from(lib);
            lu_t     &LU = cs.LU; assert(LU.capacity()>=M);
            vector_t &C = cs.C;
            
            //__________________________________________________________________
            //
            //
            // no species...
            //
            //__________________________________________________________________
            if(M<=0)
            {
                return;
            }
            
            //__________________________________________________________________
            //
            //
            // some species
            //
            //__________________________________________________________________
            matrix_t P;
            vector_t Lam;
            cs.scale_all(t);
            
            //__________________________________________________________________
            //
            //
            // No constraints
            //
            //__________________________________________________________________
            if(N<=0)
            {
                P   .make(M,M);
                Lam .make(M,zero);
                fill(P,Lam);
                if(!LU.build(P))
                    throw exception("invalid full constraints");
                mkl::set(C,Lam);
                LU.solve(P,C);
                for(size_t i=M;i>0;--i)
                {
                    if(C[i]<=numeric<double>::tiny) C[i] = 0;
                }
                return;
            }
            
            //__________________________________________________________________
            //
            //
            // some constraints
            //
            //__________________________________________________________________
            P.make(Nc,M);
            Lam.make(Nc,zero);
            fill(P,Lam);
            
            std::cerr << "P="   << P   << std::endl;
            std::cerr << "Lam=" << Lam << std::endl;
            
            //__________________________________________________________________
            //
            //
            // check sanity
            //
            //__________________________________________________________________
            matrix_t P2(Nc,Nc);
            lu_t     L2(Nc);
            
            mkl::mul_rtrn(P2, P, P);
            if( !L2.build(P2) )
                throw exception("singular set of constraints");
            
            //__________________________________________________________________
            //
            //
            // compute Xstar
            //
            //__________________________________________________________________
            vector_t Mu(Nc,zero);
            mkl::set(Mu,Lam);
            L2.solve(P2, Mu);
            vector_t Xstar(M,zero);
            mkl::mul_trn(Xstar, P, Mu);
            std::cerr << "Xstar=" << Xstar << std::endl;
            
            
            //__________________________________________________________________
            //
            //
            // compute Q
            //
            //__________________________________________________________________
            matrix_t Q(N,M);
            {
                matrix_t F(M,M);
                for(size_t i=Nc;i>0;--i)
                {
                    for(size_t j=M;j>0;--j)
                    {
                        F[j][i] = P[i][j];
                    }
                }
                vector_t __W(M,zero);
                matrix_t __V(M,M);
                if( !SVD::build(F, __W, __V))
                    throw exception("singular constraints: no orthonormal basis");
                for(size_t i=N;i>0;--i)
                {
                    for(size_t j=M;j>0;--j)
                    {
                        Q[i][j] = F[j][i+Nc];
                    }
                }
            }
            std::cerr << "Q=" << Q << std::endl;
            
#define RECOMPUTE_C() do {  mkl::mul(V,Q,C); mkl::mul_trn(C,Q,V); mkl::add(C,Xstar); } while(false)
            vector_t V(N,zero);
            vector_t &Gamma = cs.Gamma;
            matrix_t &W     = cs.W;
            matrix_t &Phi   = cs.Phi;
            vector_t &dC    = cs.dC;
            vector_t  X(M,zero);
            
            
            //__________________________________________________________________
            //
            //
            // generate starting point
            //
            //__________________________________________________________________
        INITIALIZE:
            if( !cs.trial(ran,t) )
            {
                std::cerr << "invalid trial" << std::endl;
                exit(1);
            }
            std::cerr << "C0=" << C << std::endl;
            RECOMPUTE_C();
            std::cerr << "C1=" << C << std::endl;
            
            size_t count = 0;
            
        NEWTON_STEP:
            ++count;
            //-- todo: count max=> bad conc: retry
            if(count>cs.MAX_NEWTON_STEPS)
            {
                std::cerr << "Not Converged: Retry" << std::endl;
                goto INITIALIZE;
            }
            
            //-- save C
            mkl::set(X,C);
            
            //-- compute the Newton's step mark II
            cs.compute_Gamma_and_Phi(t,false);
            std::cerr << "Gamma=" << Gamma << std::endl;
            const double H0 = cs.Gamma2RMS();
            std::cerr << "H0=" << H0 << std::endl;
            mkl::mul_rtrn(W,Phi,Q);
            if( !LU.build(W) )
            {
                std::cerr << "Invalid compositon" << std::endl;
                goto INITIALIZE;
            }
            
            //-- compute dV in V...
            mkl::neg(V,Gamma);
            LU.solve(W,V);
            
            //-- compute dC
            mkl::mul_trn(dC, Q, V);
            
            std::cerr << "dC0=" << dC << std::endl;
            
            //-- dC is a decreasing step for |Gamma|
            mkl::add(C,dC);
            RECOMPUTE_C();
            double H1    = cs.compute_rms(t);
            double alpha = 1;
            bool   cut   = false;
            while(H1>H0)
            {
                cut = true;
                alpha *= 0.1;
                if( alpha < numeric<double>::ftol )
                {
                    std::cerr << "Spurious Point" << std::endl;
                    goto FINALIZE;
                }
                mkl::set(C,X);
                mkl::muladd(C,alpha,dC);
                RECOMPUTE_C();
                //std::cerr << "alpha=" << alpha << "; C=" << C << std::endl;
                H1 = cs.compute_rms(t);
            }
            
            if(cut)
                goto NEWTON_STEP;
            
            // effective dC
            mkl::set(dC,X);
            mkl::sub(dC,C);
            std::cerr << "dC1=" << dC << std::endl;
            
            // test convergence
            for(size_t i=M;i>0;--i)
            {
                double err = fabs(dC[i]);
                if(err<=numeric<double>::tiny) err = 0;
                if(err>numeric<double>::ftol*fabs(C[i]))
                {
                    std::cerr << "Not converged" << std::endl;
                    goto NEWTON_STEP;
                }
            }
            
            std::cerr << "Converged" << std::endl;
            
        FINALIZE:
            std::cerr << "C=" << C << std::endl;
            
            //-- non linear error
            cs.compute_Gamma_and_Phi(t,false);
            mkl::mul_rtrn(W,Phi,Q);
            if( !LU.build(W) )
            {
                std::cerr << "Invalid Final Compositon: Retry" << std::endl;
                goto INITIALIZE;
            }
            //-- compute dV in V...
            mkl::neg(V,Gamma);
            LU.solve(W,V);
            
            //-- compute error in dC
            mkl::mul_trn(dC, Q, V);
            for_each(dC.begin(), dC.end(), numeric<double>::round_error);
            std::cerr << "Cer1=" << dC << std::endl;
            
            for(size_t i=M;i>0;--i)
            {
                if(fabs(C[i])<=dC[i]) C[i] = 0;
            }
            std::cerr << "C1=" << C << std::endl;
            
            //-- linear error
            mkl::mul(Mu,P,C);
            mkl::sub(Mu,Lam);
            L2.solve(P2, Mu);
            mkl::mul_trn(dC, P, Mu);
            for_each(dC.begin(), dC.end(), numeric<double>::round_error);
            std::cerr << "Cer2=" << dC << std::endl;

            //-- get rid of negative value
            for(size_t i=M;i>0;--i)
            {
                if(C[i]<=dC[i]) C[i] = 0;
            }
            
            if(!cs.normalize_C(t))
            {
                goto INITIALIZE;
            }
            
            //-- OK, check RMS
            mkl::mul(Mu,P,C);
            mkl::sub(Mu,Lam);
            L2.solve(P2, Mu);
            mkl::mul_trn(dC, P, Mu);
            const double rms = mkl::rms(dC);
            std::cerr << "RMS=" << rms << std::endl;
            
        }
        
        
    }
}
