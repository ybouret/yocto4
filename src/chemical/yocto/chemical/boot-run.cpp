#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"
//#include "yocto/ios/ocstream.hpp"
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
            static inline void generate_starting(equilibria          &cs,
                                                 const array<double> &X0,
                                                 urand32             &ran,
                                                 const double        &t)
            {
                assert(X0.size()==cs.C.size());
                mkl::set(cs.C,X0);
                cs.trial(ran,t);
            }
            
            
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
            //__________________________________________________________________
            //
            // always restore topology upon return
            //
            //__________________________________________________________________
            
            auto_clean<equilibria> onReturn( cs, & equilibria::restore_topology );
            
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
            // some species: M>0
            //
            //__________________________________________________________________
            cs.scale_all(t);
            lu_t     LU(M);
            matrix_t P;
            
            //__________________________________________________________________
            //
            //
            // no equilibrium
            //
            //__________________________________________________________________
            if(N<=0)
            {
                assert(Nc==M);
                //------------------------------------------------------
                // the P matrix is square
                //------------------------------------------------------
                P.make(M,M);
                fill(P, cs.C);
                std::cerr << "P="   << P << std::endl;
                std::cerr << "Lam=" << cs.C << std::endl;
                if( !LU.build(P) )
                    throw exception("invalid chemical constraints");
                LU.solve(P,cs.C);
                cs.cleanup_C();
                return;
            }
            
            //__________________________________________________________________
            //
            //
            // some equilibria: compute #DOF and #FIX
            //
            //__________________________________________________________________
            vector_t     Lam;
            vector_t     X0(M,0.0);
            
            const size_t dof   = dispatch(P, Lam, cs.fixed, X0);
            const size_t fix   = Nc-dof;
            std::cerr << "#DOF  =" << dof << std::endl;
            std::cerr << "#FIX  =" << fix << std::endl;
            std::cerr << "fixed =" << cs.fixed << std::endl;
            std::cerr << "X0    =" << X0    << std::endl;
            
            //__________________________________________________________________
            //
            //
            // Biased stoichiometry
            //
            //__________________________________________________________________
            cs.update_topology();
            std::cerr << "nu=" << cs.nu << std::endl;
            std::cerr << "Nu=" << cs.Nu << std::endl;
            
            //__________________________________________________________________
            //
            //
            // No DOF: just a biased chemistry
            //
            //__________________________________________________________________
            if(dof<=0)
            {
                generate_starting(cs,X0, ran, t);
                //std::cerr << "C0=" << cs.C << std::endl;
                cs.cleanup_C();
                cs.normalize_C(t);
                cs.restore_topology();
                cs.normalize_C(t); // just to check
                std::cerr << "C=" << cs.C << std::endl;
                return;
            }
            
            
            //__________________________________________________________________
            //
            //
            // Most generic case: keep the altered topology and make
            // some algebra
            //
            //__________________________________________________________________
            std::cerr << "P="   << P   << std::endl;
            std::cerr << "Lam=" << Lam << std::endl;
            vector_t Xstar(M,0.0);
            matrix_t Z(M,N);
            
            
            //==================================================================
            // check rank of P
            //==================================================================
            matrix_t  P2(dof,dof);
            lu_t      LU2(dof);
            mkl::mul_rtrn(P2, P, P);
            if( !LU2.build(P2))
                throw exception("singular chemical constraints/rank");
            vector_t Mu(dof,0.0);
            
            //__________________________________________________________________
            //
            //
            // Compute the Z matrix: C = Xstar + Z*U
            //
            //__________________________________________________________________
            {
                
                //==============================================================
                //
                // compute the restriction matrices A and B
                //
                //==============================================================
                const size_t na = M-fix;
                matrix_t A(na,M);           // restriction matrix
                matrix_t B;                 // complement matrix
                if(fix>0) B.make(fix,M);
                {
                    vector<size_t> indices(na,as_capacity);
                    for(size_t j=1;j<=M;++j)
                    {
                        if( !cs.fixed[j] ) indices.push_back(j);
                    }
                    std::cerr << "indices=" << indices << std::endl;
                    assert(na==indices.size());
                    for(size_t i=1;i<=na;++i)
                    {
                        A[i][ indices[i] ] = 1;
                    }
                    
                    if(fix>0)
                    {
                        indices.free();
                        for(size_t j=1;j<=M;++j)
                        {
                            if( cs.fixed[j] ) indices.push_back(j);
                        }
                        assert(fix==indices.size());
                        for(size_t i=1;i<=fix;++i)
                        {
                            B[i][indices[i]] = 1;
                        }
                    }
                }
                //std::cerr << "A=" << A << std::endl;
                //std::cerr << "B=" << B << std::endl;
                
                //==============================================================
                //
                // check the constraints compatibility and finalize Xstar
                //
                //==============================================================
                {
                    matrix_t Psi(dof,na);
                    mkl::mul_rtrn(Psi, P, A);
                    //std::cerr << "Psi=" << Psi << std::endl;
                    matrix_t Psi2(dof,dof);
                    mkl::mul_rtrn(Psi2, Psi, Psi);
                    if(!LU.build(Psi2))
                        throw exception("incompatible chemical constraints");
                    mkl::mul(Mu,P,X0);
                    mkl::subp(Mu,Lam);
                    LU.solve(Psi2, Mu);
                    vector_t Y0(na,0.0);
                    mkl::mul_trn(Y0, Psi, Mu);
                    //std::cerr << "Y0=" << Y0 << std::endl;
                    mkl::mul_trn(Xstar, A, Y0);
                    //std::cerr << "X1=" << Xstar << std::endl;
                }
                mkl::add(Xstar,X0);
                std::cerr << "Xstar=" << Xstar << std::endl;
                
                //==================================================================
                //
                // Compute Z by orthogonal space to B' \oplus P'
                //
                //==================================================================
                {
                    //--------------------------------------------------------------
                    //-- create B+P subspace
                    //--------------------------------------------------------------
                    matrix_t F(M,Nc);
                    for(size_t j=1;j<=B.rows;++j)
                    {
                        for(size_t i=1;i<=M;++i)
                        {
                            F[i][j] = B[j][i];
                        }
                    }
                    
                    for(size_t j=1;j<=P.rows;++j)
                    {
                        for(size_t i=1;i<=M;++i)
                        {
                            F[i][j+B.rows] = P[j][i];
                        }
                    }
                    
                    //--------------------------------------------------------------
                    //-- check rank
                    //--------------------------------------------------------------
                    //std::cerr << "F=" << F << std::endl;
                    if( !has_maximal_rank(F,LU))
                    {
                        throw exception("singular chemical constraint/orthogonal space");
                    }
                    
                    //--------------------------------------------------------------
                    //-- make an orthonormal space from F
                    //--------------------------------------------------------------
                    matrix_t G(M,M);
                    for(size_t j=1;j<=Nc;++j)
                    {
                        for(size_t i=1;i<=M;++i)
                        {
                            G[i][j] = F[i][j];
                        }
                    }
                    
                    vector_t __W(M,0);
                    matrix_t __V(M,M);
                    if( ! SVD::build(G, __W, __V) )
                        throw exception("singular chemical constraint/SVD");
                    
                    for(size_t j=1;j<=N;++j)
                    {
                        for(size_t i=1;i<=M;++i)
                        {
                            Z[i][j] =  G[i][j+Nc];
                        }
                    }
                    
                }
            }
            std::cerr << "Z=" << Z << std::endl;
            
            //__________________________________________________________________
            //
            //
            // Le us find some concentrations
            //
            //__________________________________________________________________
            const size_t        min_steps         = STEPS_PER_SPECIES * M;
            vector_t U(N,0.0);
            vector_t &C  = cs.C;
            vector_t &dC = cs.dC;
            vector_t  X(M,0.0);
            
            size_t    count  =  0;
            double    oldRMS = -1;
            size_t    trials =  0;
            
#define COMPUTE_C(toto) do {\
mkl::set(toto,Xstar); mkl::muladd(toto,Z,U);\
for(size_t ii=M;ii>0;--ii) if(cs.fixed[ii]) toto[ii] = X0[ii]; \
} while(false)
            
            
        INITIALIZE:
            generate_starting(cs, X0, ran, t);
            //std::cerr << "C0=" << C << std::endl;
            
            count  =  0;
            oldRMS = -1;
            ++trials;
            if( trials > MAX_TRIALS)
                throw exception("unable to find composition after MAX_TRIALS: check constraints...");
            
        PROJECTION:
            std::cerr << std::endl;
            std::cerr << "C=" << C << std::endl;
            
            //------------------------------------------------------------------
            //-- compute projected U
            //------------------------------------------------------------------
            mkl::mul_trn(U,Z,C);
            //std::cerr << "U=" << U << std::endl;
            
            //------------------------------------------------------------------
            //-- compute projected concentration in dC
            //------------------------------------------------------------------
            COMPUTE_C(dC);
            std::cerr << "C =" << C  << std::endl;
            std::cerr << "Cp=" << dC << std::endl;
            
            //------------------------------------------------------------------
            //-- compute virtual source term
            //------------------------------------------------------------------
            mkl::sub(dC,C);
            std::cerr << "dC0=" << dC << std::endl;
            
            //------------------------------------------------------------------
            //-- legalize trial move
            //------------------------------------------------------------------
            //cs.legalize_dC(t);
            std::cerr << "dC1=" << dC << std::endl;
            
            //------------------------------------------------------------------
            //-- save C
            //------------------------------------------------------------------
            mkl::set(X,C);
            
            //------------------------------------------------------------------
            //-- move C towards projection
            //------------------------------------------------------------------
            mkl::add(C,dC);
            
            //------------------------------------------------------------------
            //-- normalize it
            //------------------------------------------------------------------
            cs.cleanup_C();
            std::cerr  << "Cn=" << cs.C << std::endl;
            
#if 1
            if(!cs.normalize_C(t))
            {
                std::cerr << "invalid composition: try again..." << std::endl;
                goto INITIALIZE;
            }
            std::cerr << "C=" << C << std::endl;
#endif
            
            //------------------------------------------------------------------
            //-- compute effective difference
            //------------------------------------------------------------------
            mkl::sub(X,C);
            
            //std::cerr << "dX=" << X << std::endl;
            
            //------------------------------------------------------------------
            //-- compute distance to previous concentrations
            //------------------------------------------------------------------
            const double RMS = mkl::rms(X);
            //fp("%g %g\n", double(count), RMS );
           
            //------------------------------------------------------------------
            // test convergence when allowable
            //------------------------------------------------------------------
            if(++count>min_steps)
            {
                assert(oldRMS>=0);
                if(RMS>=oldRMS)
                    goto FINALIZE;
            }
            
            oldRMS = RMS;
            goto PROJECTION;
            
        FINALIZE:
            std::cerr << "C1=" << C << std::endl;
            
            //__________________________________________________________________
            //
            // compute the residue from the #D.O.F.
            //__________________________________________________________________
            
            //------------------------------------------------------------------
            // dC = P'*(P*P')^(-1)*(P*X-Lam)
            //------------------------------------------------------------------
            mkl::mul(Mu,P,C);
            mkl::sub(Mu,Lam);
            LU2.solve(P2,Mu);
            mkl::mul_trn(dC,P,Mu);
            std::cerr << "R=" << dC << std::endl;
            
            //------------------------------------------------------------------
            // normalized rms
            //------------------------------------------------------------------
            const double rms = mkl::rms(dC);
            std::cerr << "rms=" << rms << std::endl;
            if( rms > math::numeric<double>::ftol )
                throw exception("inconsistent chemical constraints/RMS is too big");
            
            //------------------------------------------------------------------
            // check rounding
            //------------------------------------------------------------------
            for(size_t i=M;i>0;--i)
            {
                double err = fabs(dC[i]);
                if(err<=numeric<double>::tiny)
                    err=0;
                else
                    err = pow(10.0,ceil(log10(err)));
                dC[i] = err;
            }
            std::cerr << "R1=" << dC << std::endl;
            
            //------------------------------------------------------------------
            // round
            //------------------------------------------------------------------
            for(size_t i=M;i>0;--i)
            {
                if(C[i]<=max_of<double>(numeric<double>::tiny,dC[i]))
                    C[i] = 0;
            }
            std::cerr << "C2=" << C << std::endl;
            
            //------------------------------------------------------------------
            // partial normalization
            //------------------------------------------------------------------
            if( ! cs.normalize_C(t) )
                throw exception("invalid final composition/partial topology");
            std::cerr << "C3=" << C << std::endl;
            
            //------------------------------------------------------------------
            // full normalization
            //------------------------------------------------------------------
            cs.restore_topology();
            if( ! cs.normalize_C(t) )
                throw exception("invalid final composition/full topology");
            std::cerr << "C4=" << C << std::endl;
            
        }
        
        
    }
}
