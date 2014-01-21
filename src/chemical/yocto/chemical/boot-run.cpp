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
        
#if 0
        namespace {
            
            static inline
            void set_fixed(vector_t        &C,
                           const vector_t  &X0,
                           const uvector_t &ifix) throw()
            {
                assert(C.size()==X0.size());
                for(size_t i=ifix.size();i>0;--i)
                {
                    assert(ifix[i]>0);
                    assert(ifix[i]<=C.size());
                    const size_t k = ifix[i];
                    C[k] = X0[k];
                }
            }
            
        }
#endif
        
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
        
#if 0
        static inline bool has_fixed( const size_t j, const vector<size_t> &fixed ) throw()
        {
            assert(j>0);
            for(size_t i=fixed.size();i>0;--i)
            {
                if(j==fixed[i])
                    return true;
            }
            return false;
        }
#endif
        
        static inline
        void collect_fixed(vector<size_t> &fixed,
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
            
            // second pass: gather fixed indices and concentration
            for(size_t i=1;i<=Nc;++i)
            {
                size_t k = 0;
                if(is_fixed(A[i],k))
                {
                    fixed.push_back(k);
                    Cf[k] = B[i];
                }
            }
            
            quicksort(fixed);
            
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
            
            collect_fixed(cs.fixed,
                          cs.Cf,
                          A,
                          B);
            
            const array<size_t> &fixed = cs.fixed;
            const array<double> &Cf    = cs.Cf;
            
            std::cerr << "A=" << A << std::endl;
            std::cerr << "B=" << B << std::endl;
            
            std::cerr << "fixed=" << fixed << std::endl;
            std::cerr << "Cf   =" << Cf    << std::endl;
            exit(0);
            
#if 0
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
            if(!SVD::orthonormal(Q,P))
                throw exception("chemical::boot: invalid constraints/SVD");
            std::cerr << "Q=" << Q << std::endl;
            
            //__________________________________________________________________
            //
            //
            // alter topology/fixed constraints
            //
            //__________________________________________________________________
            const vector_t  &Cf    = cs.Cf;
            const uvector_t &fixed = cs.fixed;
            find_fixed(cs.Cf, cs.fixed);
            std::cerr << "fixed = " <<  fixed  << std::endl;
            std::cerr << "Cf    = " <<  Cf     << std::endl;
            
            cs.fixed_topology();
            std::cerr << "nu=" << cs.nu << std::endl;
            std::cerr << "Nu=" << cs.Nu << std::endl;
            
            matrix_t K;
            find_kernel(K,M);
            const size_t nk = K.rows;
            std::cerr << "K=" << K << std::endl;
            if(nk>0)
            {
                std::cerr << "Z=K'*inv(K*K')*K" << std::endl;
            }
            
            
            //__________________________________________________________________
            //
            //
            // Newton mark-II
            //
            //__________________________________________________________________
            
            
#define RECOMPUTE_C() do {  cs.fixed_C(); mkl::mul(V,Q,C); mkl::mul_trn(C,Q,V); mkl::add(C,Xstar); cs.fixed_C(); } while(false)
            
            vector_t V(N,zero);
            vector_t &Gamma = cs.Gamma;
            matrix_t &W     = cs.W;
            matrix_t &Phi   = cs.Phi;
            vector_t &dC    = cs.dC;
            vector_t  X(M,zero);
            const size_t max_trials = M * MAX_TRIALS_PER_SPECIES;
            size_t       trials     = 0;
            
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
            std::cerr << "C0=" << C << std::endl;
            
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
#endif
            
        }
        
        
    }
}
