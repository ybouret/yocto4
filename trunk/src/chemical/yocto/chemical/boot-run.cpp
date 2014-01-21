#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/kernel/svd.hpp"

#include "yocto/ios/ocstream.hpp"
#include "yocto/auto-clean.hpp"

#include "yocto/sort/quick.hpp"

namespace yocto
{
    namespace chemical
    {
        
        using namespace math;
        typedef algebra<double> mkl;
        typedef svd<double>     SVD;
        
        
        static inline bool is_fixed( const array<ptrdiff_t> &u, size_t &j ) throw()
        {
            j=0;
            size_t nf = 0;
            for(size_t i=u.size();i>0;--i)
            {
                if( u[i] != 0 )
                {
                    j=i;
                    ++nf;
                }
            }
            assert(nf>0);
            if(1==nf)
                return true;
            else
            {
                j=0;
                return false;
            }
        }
        
        
        static inline
        void collect_fixed(uvector_t      &fixed,
                           array<double>  &Cf,
                           imatrix_t      &A,
                           vector_t       &B )
        {
            assert(A.rows==B.size());
            assert(A.cols==Cf.size());
            const size_t M  = A.cols;
            const size_t Nc = A.rows;
            
            fixed.free();
            for(size_t i=M;i>0;--i) Cf[i] = 0;
            
            // first pass: normalize it
            bool modified = false;
            do
            {
                modified = false;
                for(size_t i=1;i<=Nc;++i)
                {
                    size_t k = 0;
                    if(is_fixed(A[i],k))
                    {
                        assert(k>0);
                        assert(k<=M);
                        assert(A[i][k]!=0);
                        
                        B[i]   /= A[i][k]; //! normalize value
                        A[i][k] = 1;       //! normalize constraint
                        
                        if(B[i]<0)
                            throw exception("fixed constraint will set negative concentration!");
                        
                        // do we impact other constraints ?
                        for(size_t j=1;j<=Nc;++j)
                        {
                            if(j!=i)
                            {
                                if(A[j][k]!=0)
                                {
                                    modified = true;
                                    B[j]    -= B[i] * A[j][k];
                                    A[j][k]  = 0;
                                }
                            }
                        }
                    }
                }
            }
            while(modified);
            
            // second pass: check no zero constraint !
            for(size_t i=1; i<=Nc;++i)
            {
                size_t nz = 0;
                for(size_t j=M;j>0;--j)
                {
                    if(A[i][j]!=0) ++nz;
                }
                if(nz<=0)
                    throw exception("multiple fixed constraints detected");
            }
            
            
            // third pass: gather fixed indices and concentration
            for(size_t i=1;i<=Nc;++i)
            {
                size_t k = 0;
                if(is_fixed(A[i],k))
                {
                    if(!fixed.insert(k))
                        throw exception("unexpected multipled fixed constraint");
                    Cf[k] = B[i];
                }
            }
            
            
        }
        
        static inline
        void collect_combi(matrix_t        &P,
                           vector_t        &Lam,
                           matrix_t        &K,
                           const imatrix_t &A,
                           const vector_t  &B)
        {
            
            
            assert(A.rows==B.size());
            
            const size_t M  = A.cols;
            const size_t Nc = A.rows;
            
            vector<size_t> proj(Nc,as_capacity);
            vector<size_t> kern(Nc,as_capacity);
            
            for(size_t i=1;i<=Nc;++i)
            {
                size_t k = 0;
                if(is_fixed(A[i],k))
                    continue;
                if(fabs(B[i])<=0)
                    kern.push_back(i);
                proj.push_back(i);
            }
            
            const size_t np = proj.size();
            if(np>0)
            {
                P   .make(np,M);
                Lam .make(np,0.0);
                for(size_t i=1;i<=np;++i)
                {
                    const size_t p = proj[i];
                    for(size_t j=M;j>0;--j) P[i][j] = A[p][j];
                    Lam[i] = B[p];
                }
            }
            
            const size_t nk = kern.size();
            if(nk>0)
            {
                K.make(nk,M);
                for(size_t i=1;i<=nk;++i)
                {
                    const size_t k = kern[i];
                    for(size_t j=M;j>0;--j) K[i][j] = A[k][j];
                }
            }
            
        }
        
        
        
        void boot::loader::operator()(equilibria &cs, collection &lib, double t)
        {
            
            static const double zero = numeric<double>::zero;
            
            auto_clean<equilibria> onReturn( cs, &equilibria::reset_topology);
            
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
            // parameters
            //__________________________________________________________________
            const size_t max_trials = M * MAX_TRIALS_PER_SPECIES;
            
            //__________________________________________________________________
            //
            //
            // Acquiring resources for equilibria
            //
            //__________________________________________________________________
            cs.build_from(lib);
            lu_t     &LU = cs.LU; assert(LU.size()>=M);
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
            cs.scale_all(t);
            
            imatrix_t A;
            
            
            
            
            //__________________________________________________________________
            //
            //
            // No reactions
            //
            //__________________________________________________________________
            if(N<=0)
            {
                A   .make(M,M);
                fill(A,C);
                matrix_t F(M,M);
                for(size_t i=0;i<A.size;++i)
                {
                    F(i) = A(i);
                }
                if(!LU.build(F))
                    throw exception("invalid full constraints");
                lu_t::solve(F,C);
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
            A.make(Nc,M);
            vector_t B(Nc,zero);
            fill(A,B);
            std::cerr << "A0=" << A << std::endl;
            std::cerr << "B0=" << B << std::endl;
            
            
            //__________________________________________________________________
            //
            //
            // Clean-up constraints
            //
            //__________________________________________________________________
            collect_fixed(cs.fixed,
                          cs.Cf,
                          A,
                          B);
            
            const uvector_t     &fixed = cs.fixed;
            const array<double> &Cf    = cs.Cf;
            
            std::cerr << "A=" << A << std::endl;
            std::cerr << "B=" << B << std::endl;
            
            std::cerr << "fixed=" << fixed << std::endl;
            std::cerr << "Cf   =" << Cf    << std::endl;
            
            cs.fixed_topology();
            std::cerr << "nu=" << cs.nu << std::endl;
            std::cerr << "Nu=" << cs.Nu << std::endl;
            
            //__________________________________________________________________
            //
            //
            // All-fixed constraints
            //
            //__________________________________________________________________
            const size_t nf = fixed.size();
            if( nf == Nc )
            {
                std::cerr << "-- all fixed constraints" << std::endl;
                for(size_t iter=1;iter<=max_trials;++iter)
                {
                    if(cs.trial(ran,t))
                        return;
                }
                throw exception("unable to solved all fixed constraints");
            }
            
            //__________________________________________________________________
            //
            //
            // Remaining constraints
            //
            //__________________________________________________________________
            matrix_t P;
            vector_t Lam;
            matrix_t K;
            
            collect_combi(P, Lam, K, A, B);
            
            std::cerr << "P="   << P << std::endl;
            std::cerr << "Lam=" << Lam << std::endl;
            std::cerr << "K="   << K << std::endl;
            
            
            
            const size_t Mf = M-nf;
            matrix_t Psi(M,Mf);
            {
                size_t k=0;
                for(size_t j=1;j<=M;++j)
                {
                    if( fixed.search(j) )
                    {
                        continue;
                    }
                    else
                    {
                        ++k;
                        assert(k<=Mf);
                        Psi[j][k] = 1;
                    }
                }
            }
            std::cerr << "Psi=" << Psi << std::endl;
            const size_t np = P.rows;
            matrix_t P2(np,np);
            mkl::mul_rtrn(P2, P, P);
            if( !LU.build(P2))
                throw exception("singular projection constraints set !");
            
            matrix_t alpha(np,Mf);
            mkl::mul(alpha,P,Psi);
            std::cerr << "alpha=" << alpha << std::endl;
            
            //__________________________________________________________________
            //
            //
            // check rank and compute Xstar
            //
            //__________________________________________________________________
            vector_t Xstar(M,zero);
            vector_t Mu(np,zero);    // temporary for linear constraint
            {
                matrix_t alpha2(np,np);
                mkl::mul_rtrn(alpha2,alpha,alpha);
                if(!LU.build(alpha2))
                    throw exception("singular constraints set!");
                mkl::set(Mu,Lam);
                mkl::mulsub(Mu, P, Cf);
                lu_t::solve(alpha2, Mu);
                vector_t Ystar(Mf,zero);
                mkl::mul_trn(Ystar, alpha, Mu);
                mkl::mul(Xstar,Psi,Ystar);
                std::cerr << "X1=" << Xstar << std::endl;
                mkl::add(Xstar,Cf);
            }
            std::cerr << "Xstar=" << Xstar << std::endl;
            
            //__________________________________________________________________
            //
            //
            // Finding matrices for solver
            //
            //__________________________________________________________________
            matrix_t beta(N,Mf);
            if(! SVD::orthonormal(beta,alpha) )
                throw exception("Unable to find and orthornornmal basis");
            
            std::cerr << "beta=" << beta << std::endl;
            
            matrix_t Theta(N,M);
            mkl::mul_rtrn(Theta,beta,Psi);
            std::cerr << "Theta=" << Theta << std::endl;
            
            //__________________________________________________________________
            //
            //
            // Algorithm
            //
            //__________________________________________________________________
            vector_t &CC = cs.CC;
            vector_t &dV    = cs.xi;
            matrix_t  W     = cs.W;
            matrix_t &Phi   = cs.Phi;
            vector_t &Gamma = cs.Gamma;
            vector_t &dC    = cs.dC;
            
            
            vector_t   V(N,zero);
            vector_t   X(M,zero);

#define RECOMPUTE_C() do { mkl::set(CC,C); mkl::sub(CC,Xstar); mkl::mul(V,Theta,CC); mkl::set(C,Xstar); mkl::muladd_trn(C,Theta,V); } while(false)
            
            //==================================================================
            //
            // init a trial
            //
            //==================================================================

            size_t trials = 0;
        INITIALIZE:
            ++trials;
            if(trials>max_trials)
            {
                throw exception("unable to solve system");
            }
            
            if( !cs.trial(ran,t) )
            {
                std::cerr << "invalid initial concentration" << std::endl;
                goto INITIALIZE;
            }
            
            RECOMPUTE_C();
            
            //==================================================================
            //
            // Perform a Newton's Step
            //
            //==================================================================
            size_t count = 0;
        NEWTON_STEP:
            ++count;
            if(count>equilibria::MAX_NEWTON_STEPS)
            {
                std::cerr << "-- Newton-II: not converged" << std::endl;
                goto INITIALIZE;
            }

            //------------------------------------------------------------------
            // numerical step
            //------------------------------------------------------------------
            cs.compute_Gamma_and_Phi(t,false);
            const double H0 = cs.Gamma2RMS();      // approximate norm
            mkl::mul_rtrn(W,Phi,Theta);
            if(!LU.build(W))
            {
                std::cerr << "-- Newton-II: invalid composition" << std::endl;
                goto INITIALIZE;
            }
            mkl::neg(dV,Gamma);
            lu_t::solve(W,dV);
            mkl::mul_trn(dC,Theta,dV);
            
            //------------------------------------------------------------------
            // test numerical behaviour: dC is a descent direction for |Gamma|
            //------------------------------------------------------------------
            mkl::set(X,C);
            mkl::add(C,dC);
            RECOMPUTE_C(); // numerical errors
            double H1  = cs.compute_rms(t);
            double fac = 1.0;
            bool   cut = false;
            while( H1 > H0 )
            {
                cut  = true;
                fac *= 0.1;
                if(fac < numeric<double>::ftol )
                {
                    std::cerr << "-- Newton II: spurious point" << std::endl;
                    goto FINALIZE;
                }
                mkl::set(C,X);
                mkl::muladd(C,fac,dC);
                RECOMPUTE_C();
                H1 = cs.compute_rms(t);
            }
            
            if(cut)
                goto NEWTON_STEP; //! won't stop on a partial step
            
            //------------------------------------------------------------------
            // Effective Step
            //------------------------------------------------------------------
            mkl::sub(X,C);
           
            //------------------------------------------------------------------
            // Test convergence
            //------------------------------------------------------------------
            for(size_t i=M;i>0;--i)
            {
                double err = fabs(X[i]);
                if( err < numeric<double>::tiny ) err = 0;
                if( err > numeric<double>::ftol * fabs(C[i]))
                    goto NEWTON_STEP;
            }
            
            std::cerr << "-- Newton-II: converged" << std::endl;
            
        FINALIZE:
            std::cerr << "C=" << C << std::endl;
            
            //------------------------------------------------------------------
            // non-linear error
            //------------------------------------------------------------------
            cs.compute_Gamma_and_Phi(t,false);
            mkl::mul_rtrn(W,Phi,Theta);
            if( ! LU.build(W) )
            {
                std::cerr << "-- Newton II: invalid final composition" << std::endl;
                goto INITIALIZE;
            }
            mkl::neg(dV,Gamma);
            lu_t::solve(W,dV);         // error in V
            mkl::mul_trn(dC,Theta,dV); // error in C
            
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
            lu_t::solve(P2, Mu);
            mkl::mul_trn(dC, P, Mu);
            for_each(dC.begin(), dC.end(), numeric<double>::round_error);
            
            //------------------------------------------------------------------
            //-- get rid of negative value
            //------------------------------------------------------------------
            for(size_t i=M;i>0;--i)
            {
                if(C[i]<=dC[i]) C[i] = 0;
            }
            
            std::cerr << "C=" << C << std::endl;
            
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
            lu_t::solve(P2, Mu);
            mkl::mul_trn(dC, P, Mu);
            const double rms = mkl::rms(dC);
            std::cerr << "RMS=" << rms << std::endl;
            if(rms>numeric<double>::ftol)
            {
                throw exception("ill-conditionned chemical constraints");
            }

            
        }
        
        
    }
}
