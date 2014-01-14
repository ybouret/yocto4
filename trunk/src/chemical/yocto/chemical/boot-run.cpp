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
            
            //if(VALUE<=numeric<double>::tiny) VALUE = 0;    }
            
#define COMPUTE_C(toto) do {                     \
mkl::set(toto,Xstar); mkl::muladd(toto,Z,U);     \
for(size_t ii=M;ii>0;--ii) {                     \
double &VALUE = toto[ii];                        \
if(cs.fixed[ii])                 VALUE = X0[ii]; }\
} while(false)
            
            vector_t U(N,0.0);
            vector_t &C     = cs.C;
            vector_t &dC    = cs.dC;
            matrix_t &W     = cs.W;
            matrix_t &Phi   = cs.Phi;
            vector_t &Gamma = cs.Gamma;
            vector_t &dU    = cs.xi;
            vector_t  X(M,0.0);
            
            size_t trials=0;
            
        INITIALIZE:
            ++trials;
            std::cerr << std::endl << "Starting point #" << trials << std::endl;
            generate_starting(cs, X0, ran, t);
            
            std::cerr << "C0=" << C << std::endl;
            
            // projection
            mkl::mul_trn(U,Z,C);
            COMPUTE_C(C);
            std::cerr << "C1=" << C << std::endl;
            
            ios::ocstream fp("rms.dat",false);
            size_t count =  0;
            double RMS   = -1; //!< Test on displacement
            
        NEWTON_STEP:
            ++count;
            // newton step
            cs.compute_Gamma_and_Phi(t,false);
            const double H0 = cs.Gamma2RMS();
            mkl::mul(W,Phi,Z);
            
            std::cerr << "Gamma=" << Gamma << std::endl;
            std::cerr << "H0="    << H0    << std::endl;
            if( !LU.build(W) )
            {
                std::cerr << "invalid composition in newton's step" << std::endl;
                goto INITIALIZE;
            }
            
            mkl::neg(dU,Gamma);
            LU.solve(W,dU);
            mkl::mul(dC,Z,dU);
            
            //-- clean dC
            for(size_t i=M;i>0;--i)
            {
                if(cs.fixed[i]) dC[i] = 0;
            }
            
            // save C
            mkl::set(X,C);
            
            mkl::add(C,dC);
            mkl::mul_trn(U,Z,C);
            COMPUTE_C(C);
            std::cerr << "U ="  << U << std::endl;
            std::cerr << "dC=" << dC << std::endl;
            std::cerr << "C2=" << C << std::endl;
            double H1    = cs.compute_rms(t);
            double alpha = 1;
            bool   full  = true;
            
            while(H1>H0)
            {
                full   = false;
                alpha *= 0.1;
                if(alpha < numeric<double>::ftol)
                {
                    alpha = -1;
                    break;
                }
                mkl::set(C,X);
                mkl::muladd(C, alpha, dC);
                mkl::mul_trn(U,Z,C);
                COMPUTE_C(C);
                H1 = cs.compute_rms(t);
            }
            
            
            if(alpha<0)
            {
                std::cerr << "-- [[ Spurious Step ]]" << std::endl;
                mkl::set(C,X);
                goto FINALIZE;
            }
            
            if(full)
            {
                // effective dC
                mkl::sub(X,C);
                
                std::cerr << "dX=" << X  << std::endl;
                
                const double newRMS = mkl::rms(X);
                std::cerr << "rms=" << newRMS << std::endl;
                fp("%g %g %g\n", double(count), newRMS, H1);
                
                if( H1 <= H0 )
                {
                    // successfull Newtons' Step
                    if(RMS>=0)
                    {
                        if(newRMS>=RMS)
                        {
                            std::cerr << "-- [[ Converged ]]" << std::endl;
                            goto FINALIZE;
                        }
                    }
                    
                }
                
                RMS = newRMS;
            }
            else
                RMS = -1;
            
            goto NEWTON_STEP;
            
        FINALIZE:
            std::cerr << "C1=" << C << std::endl;

            // Error from non-linear system
            cs.compute_Gamma_and_Phi(t,false);
            mkl::mul(W,Phi,Z);
            if( !LU.build(W) )
            {
                std::cerr << "Invalid Final Conc..." << std::endl;
                goto INITIALIZE;
            }
            
            std::cerr << "Gamma=" << Gamma << std::endl;
            mkl::set(dU,Gamma);
            LU.solve(W, dU);
            mkl::mul(dC,Z,dU);
            for_each( dC.begin(), dC.end(), numeric<double>::round_error);
            std::cerr << "dCnl=" << dC << std::endl;
            for(size_t i=M;i>0;--i)
            {
                if( fabs(C[i]) <= max_of(dC[i],numeric<double>::tiny) ) C[i] = 0;
            }
            std::cerr << "Cnl=" << C << std::endl;

            
            // Error from linear system
            mkl::mul(Mu,P,C);
            mkl::sub(Mu,Lam);
            std::cerr << "Mu=" << Mu << std::endl;
            LU2.solve(P2, Mu);
            mkl::mul_trn(dC,P,Mu);
            const double algErr = mkl::rms(dC);
            std::cerr << "algebraicError=" << algErr << std::endl;
            for_each( dC.begin(), dC.end(), numeric<double>::round_error);
            std::cerr << "dClin=" << dC << std::endl;
            
            for(size_t i=M;i>0;--i)
            {
                if( fabs(C[i]) <= max_of(dC[i],numeric<double>::tiny) ) C[i] = 0;
            }
            std::cerr << "Clin=" << C << std::endl;
            
            //-- check positive conc
            for(size_t i=M;i>0;--i)
            {
                if( C[i] < 0 )
                {
                    std::cerr << "Invalid final composition" << std::endl;
                    goto INITIALIZE;
                }
            }
            
            exit(1);
            
            
            
        }
        
        
    }
}
