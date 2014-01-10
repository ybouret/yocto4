#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/ios/ocstream.hpp"
//#include "yocto/sort/quick.hpp"
#include "yocto/math/kernel/svd.hpp"
#include "yocto/auto-clean.hpp"


namespace yocto
{
    namespace chemical
    {
        
        
        typedef math::algebra<double> mkl;
        typedef math::svd<double>     SVD;
        
        
        static inline void generate_starting(equilibria          &cs,
                                             const array<double> &X0,
                                             urand32             &ran,
                                             const double        &t)
        {
            assert(X0.size()==cs.C.size());
            mkl::set(cs.C,X0);
            cs.trial(ran,t);
        }
        
        
        
        void boot::loader::operator()(equilibria &cs, collection &lib, double t)
        {
            static const double FTOL = math::numeric<double>::ftol;
            
            auto_clean<equilibria> onReturn( cs, & equilibria::restore_topology );
            
            //__________________________________________________________________
            //
            //
            // Initializing
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
            // Acquiring resources
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
            std::cerr << "#DOF  ="  << dof << std::endl;
            std::cerr << "#FIX  ="  << fix << std::endl;
            std::cerr << "fixed =" << cs.fixed << std::endl;
            std::cerr << "X0    =" << X0    << std::endl;
            cs.update_topology();
            
            //__________________________________________________________________
            //
            //
            // Biased stoichiometry
            //
            //__________________________________________________________________
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
                std::cerr << "C0=" << cs.C << std::endl;
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
            {
                matrix_t  P2(dof,dof);
                mkl::mul_rtrn(P2, P, P);
                if( !LU.build(P2 ))
                    throw exception("singular chemical constraints/rank");
            }
            
            //__________________________________________________________________
            //
            //
            // Compute the Z matrix: C = Xstar + Z U
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
                std::cerr << "A=" << A << std::endl;
                std::cerr << "B=" << B << std::endl;
                
                //==============================================================
                //
                // check the constraints compatibility and finalize Xstar
                //
                //==============================================================
                {
                    matrix_t Psi(dof,na);
                    mkl::mul_rtrn(Psi, P, A);
                    std::cerr << "Psi=" << Psi << std::endl;
                    matrix_t Psi2(dof,dof);
                    mkl::mul_rtrn(Psi2, Psi, Psi);
                    if(!LU.build(Psi2))
                        throw exception("incompatible chemical constraints");
                    vector_t Mu(dof,0.0);
                    mkl::mul(Mu,P,X0);
                    mkl::subp(Mu,Lam);
                    LU.solve(Psi2, Mu);
                    vector_t Y0(na,0.0);
                    mkl::mul_trn(Y0, Psi, Mu);
                    std::cerr << "Y0=" << Y0 << std::endl;
                    mkl::mul_trn(Xstar, A, Y0);
                    std::cerr << "X1=" << Xstar << std::endl;
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
                    {
                        matrix_t F2(Nc,Nc);
                        mkl::mul_ltrn(F2, F, F);
                        if(!LU.build(F2))
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
            vector_t U(N,0.0);
            vector_t &C  = cs.C;
            vector_t &dC = cs.dC;
            vector_t  X(M,0.0);
            size_t    count=0;
            double    oldRMS = -1;
            ios::ocstream fp("rms.dat",false);
            
#define COMPUTE_C(toto) do {\
mkl::set(toto,Xstar); mkl::muladd(toto,Z,U);\
for(size_t ii=M;ii>0;--ii) if(cs.fixed[ii]) toto[ii] = X0[ii]; \
} while(false)
            
            
        INITIALIZE:
            generate_starting(cs, X0, ran, t);
            std::cerr << "C0=" << C << std::endl;
            
            count  =  0;
            oldRMS = -1;
        PROJECTION:
            std::cerr << std::endl;
            std::cerr << "C=" << C << std::endl;
            
            //-- compute projected U
            mkl::mul_trn(U,Z,C);
            std::cerr << "U=" << U << std::endl;
            
            //-- compute projected concentration
            COMPUTE_C(dC);
            std::cerr << "Cp=" << dC << std::endl;
            
            //-- compute virtual source term
            mkl::sub(dC,C);
            std::cerr << "dC0=" << dC << std::endl;
            
            //-- legalize trial move
            cs.legalize_dC(t);
            std::cerr << "dC1=" << dC << std::endl;
            
            //-- save C
            mkl::set(X,C);
            
            //-- move C towards projection
            mkl::add(C,dC);
            std::cerr  << "Cn=" << cs.C << std::endl;
            
            //-- normalize it
            cs.cleanup_C();
            std::cerr << "Cz=" << cs.C << std::endl;
            cs.normalize_C(t);
            std::cerr << "C1=" << cs.C << std::endl;
            
            //-- compute effective difference
            mkl::sub(X,C);
            
            std::cerr << "dX=" << X << std::endl;
            const double RMS = mkl::rms(X);
            fp("%g %g\n", double(count), RMS );
            if(++count>10)
            {
                if(RMS>=oldRMS)
                    goto FINALIZE;
            }
            
            oldRMS = RMS;
            goto PROJECTION;
            
        FINALIZE:
            ;
        }
        
        
    }
}
