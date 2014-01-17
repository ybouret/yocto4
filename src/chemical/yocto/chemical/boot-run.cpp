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
            vector_t &C  = cs.C;
            
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
            // No reactions
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
            // Some reactions
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
            // check sanity via Moore-Penrose
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
            if(!SVD::orthonormal(Q, P))
                throw exception("chemical::boot: invalid constraints/SVD");
            std::cerr << "Q=" << Q << std::endl;
            
            
            vector<size_t> neg_index(M,as_capacity);
            for(size_t i=1;i<=M;++i)
            {
                if(Xstar[i]<0)
                    neg_index.push_back(i);
            }
            
            std::cerr << "neg_index=" << neg_index << std::endl;
            const size_t nn = neg_index.size();
            if(nn)
            {
                matrix_t A(nn,N);
                vector_t Xm(nn,zero);
                for(size_t i=1; i<=nn; ++i)
                {
                    const size_t k = neg_index[i];
                    for(size_t j=1;j<=N;++j)
                    {
                        A[i][j] = Q[j][k];
                    }
                    Xm[i] = -Xstar[k];
                }
                std::cerr << "A=" << A << std::endl;
                std::cerr << "Xm=" << Xm << std::endl;
                std::cerr << "V0=A'*inv(A*A')*Xm" << std::endl;
            }
        
            
            
            //__________________________________________________________________
            //
            //
            // Newton mark-II
            //
            //__________________________________________________________________
            
            
#define RECOMPUTE_C() do {  mkl::mul(V,Q,C); mkl::mul_trn(C,Q,V); mkl::add(C,Xstar); } while(false)
            vector_t V(N,zero);
            vector_t &Gamma = cs.Gamma;
            matrix_t &W     = cs.W;
            matrix_t &Phi   = cs.Phi;
            vector_t &dC    = cs.dC;
            vector_t  X(M,zero);
            const size_t max_trials = M * MAX_TRIALS_PER_SPECIES;
            size_t       trials = 0;
            
            //__________________________________________________________________
            //
            //
            // Newton, mark II
            //
            //__________________________________________________________________
            
            //==================================================================
            //
            // Generate a trial valid composition
            //
            //==================================================================
        INITIALIZE:
            if(++trials>max_trials)
                throw exception("too many chemical::boot trials (%u)", unsigned(max_trials) );
            
            if( !cs.trial(ran,t) )
            {
                std::cerr << "-- Newton-II: invalid trial composition" << std::endl;
                goto INITIALIZE;
            }
            
            RECOMPUTE_C();
            {
                size_t num_neg = 0, num_pos = 0;
                for(size_t i=M;i>0;--i)
                {
                    if( C[i] < 0 ) ++num_neg; else ++num_pos;
                }
                if(num_neg>num_pos)
                    goto INITIALIZE;
            }
            
            //std::cerr << "Cini=" << C << std::endl;
            
            //==================================================================
            //
            // Compute the full Newton's Step
            //
            //==================================================================
            size_t count = 0;
        NEWTON_STEP:
            ++count;
            if(count>cs.MAX_NEWTON_STEPS)
            {
                std::cerr << "-- Newton-II: not converged" << std::endl;
                goto INITIALIZE;
            }
            
            //------------------------------------------------------------------
            // save C
            //------------------------------------------------------------------
            mkl::set(X,C);
            
            //------------------------------------------------------------------
            // compute the Newton's step mark II
            //------------------------------------------------------------------
            cs.compute_Gamma_and_Phi(t,false);
            const double H0 = cs.Gamma2RMS();  // pseudo-norm
            mkl::mul_rtrn(W,Phi,Q);
            if( !LU.build(W) )
            {
                std::cerr << "-- Newton-II: singular composition" << std::endl;
                goto INITIALIZE;
            }
            
            //------------------------------------------------------------------
            //-- compute dV in V
            //------------------------------------------------------------------
            mkl::neg(V,Gamma);
            LU.solve(W,V);
            
            //------------------------------------------------------------------
            // compute dC
            //------------------------------------------------------------------
            mkl::mul_trn(dC, Q, V);
            
            //------------------------------------------------------------------
            // dC is a decreasing step for |Gamma|
            //------------------------------------------------------------------
            mkl::add(C,dC);
            RECOMPUTE_C();
            double H1    = cs.compute_rms(t);
            double alpha = 1;
            bool   cut   = false;
            while(H1>H0)
            {
                cut    = true;
                alpha *= 0.1;
                if( alpha < numeric<double>::ftol )
                {
                    std::cerr << "-- Newton-II: spurious point" << std::endl;
                    goto FINALIZE;
                }
                mkl::set(C,X);
                mkl::muladd(C,alpha,dC);
                RECOMPUTE_C();
                H1 = cs.compute_rms(t);
            }
            
            //------------------------------------------------------------------
            // won't stop on a partially reduced step
            //------------------------------------------------------------------
            if(cut)
                goto NEWTON_STEP;
            
            //------------------------------------------------------------------
            // compute effective dC
            //------------------------------------------------------------------
            mkl::set(dC,X);
            mkl::sub(dC,C);
            
            //------------------------------------------------------------------
            // test convergence
            //------------------------------------------------------------------
            for(size_t i=M;i>0;--i)
            {
                double err = fabs(dC[i]);
                if(err<=numeric<double>::tiny) err = 0;
                if(err>numeric<double>::ftol*fabs(C[i]))
                {
                    goto NEWTON_STEP;
                }
            }
            
            std::cerr << "-- Newton-II: converged" << std::endl;
            
            //==================================================================
            //
            // sanity checks
            //
            //==================================================================
        FINALIZE:
            std::cerr << "C=" << C << std::endl;
            
            //------------------------------------------------------------------
            // non linear error
            //------------------------------------------------------------------
            cs.compute_Gamma_and_Phi(t,false);
            mkl::mul_rtrn(W,Phi,Q);
            if( !LU.build(W) )
            {
                std::cerr << "-- Newton-II: invalid final composition" << std::endl;
                goto INITIALIZE;
            }
            
            //------------------------------------------------------------------
            // compute dV in V
            //------------------------------------------------------------------
            mkl::neg(V,Gamma);
            LU.solve(W,V);
            
            //------------------------------------------------------------------
            // compute error in dC
            //------------------------------------------------------------------
            mkl::mul_trn(dC, Q, V);
            for_each(dC.begin(), dC.end(), numeric<double>::round_error);
            
            //------------------------------------------------------------------
            // cut
            //------------------------------------------------------------------
            for(size_t i=M;i>0;--i)
            {
                if(fabs(C[i])<=dC[i]) C[i] = 0;
            }
            
            //------------------------------------------------------------------
            // linear error
            //------------------------------------------------------------------
            mkl::mul(Mu,P,C);
            mkl::sub(Mu,Lam);
            L2.solve(P2, Mu);
            mkl::mul_trn(dC, P, Mu);
            for_each(dC.begin(), dC.end(), numeric<double>::round_error);
            
            //------------------------------------------------------------------
            //-- get rid of negative value
            //------------------------------------------------------------------
            for(size_t i=M;i>0;--i)
            {
                if(C[i]<=dC[i]) C[i] = 0;
            }
            
            //------------------------------------------------------------------
            // are we consistent ?
            //------------------------------------------------------------------
            if(!cs.normalize_C(t))
            {
                std::cerr << "-- Newton-II: inconsistent final composition" << std::endl;
                goto INITIALIZE;
            }
            
            //------------------------------------------------------------------
            //-- OK, check RMS
            //------------------------------------------------------------------
            mkl::mul(Mu,P,C);
            mkl::sub(Mu,Lam);
            L2.solve(P2, Mu);
            mkl::mul_trn(dC, P, Mu);
            const double rms = mkl::rms(dC);
            std::cerr << "C="   << C   << std::endl;
            std::cerr << "RMS=" << rms << std::endl;
            if(rms>numeric<double>::ftol)
            {
                throw exception("ill-conditionned chemical constraints");
            }
            
        }
        
        
    }
}
