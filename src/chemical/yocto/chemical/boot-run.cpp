#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/math/kernel/svd.hpp"

namespace yocto
{
    namespace chemical
    {
        
        
        typedef math::algebra<double> mkl;
        
        
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
            //static const double FTOL = math::numeric<double>::ftol;
            
            //__________________________________________________________________
            //
            // Initializing
            //__________________________________________________________________
            const size_t M  = lib.size();
            const size_t N  = cs.size();
            const size_t Nc = this->size();
            if( Nc+N != M )
                throw exception("#species=%u != (#equilibria=%u+#constraints=%u)", unsigned(M), unsigned(N), unsigned(Nc) );
            
            //__________________________________________________________________
            //
            // Acquiring resources
            //__________________________________________________________________
            cs.build_from(lib);
            
            //__________________________________________________________________
            //
            // no species...
            //__________________________________________________________________
            if(M<=0)
                return;
            
            //__________________________________________________________________
            //
            // some species
            //__________________________________________________________________
            cs.scale_all(t);
            matrix_t P;
            lu_t     LU;
            
            //__________________________________________________________________
            //
            // no equilibrium
            //__________________________________________________________________
            if(N<=0)
            {
                assert(Nc==M);
                //------------------------------------------------------
                // the P matrix is square
                //------------------------------------------------------
                P.make(M,M);
                LU.ensure(M);
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
            // some equilibria: compute #DOF and #FIX
            //__________________________________________________________________
            LU.ensure(M);
            vector_t     Lam;
            vector_t     X0(M,0.0);
            
            const size_t dof   = dispatch(P, Lam, cs.fixed, X0);
            const size_t fix   = Nc-dof;
            std::cerr << "#DOF="  << dof << std::endl;
            std::cerr << "#FIX="  << fix << std::endl;
            std::cerr << "fixed=" << cs.fixed << std::endl;
            std::cerr << "X0   =" << X0    << std::endl;
            cs.update_topology();
            
            //__________________________________________________________________
            //
            // Biased stoichiometry
            //__________________________________________________________________
            std::cerr << "nu=" << cs.nu << std::endl;
            std::cerr << "Nu=" << cs.Nu << std::endl;
            
            //__________________________________________________________________
            //
            // Create a starting point
            //__________________________________________________________________
            
            //------------------------------------------------------------------
            // create a random valid concentration
            //------------------------------------------------------------------
            generate_starting(cs,X0, ran, t);
            std::cerr << "C0=" << cs.C << std::endl;
            
            
            
            //__________________________________________________________________
            //
            // No DOF: just a biased chemistry
            //__________________________________________________________________
            if(dof<=0)
            {
                cs.normalize_C(t); // just to check
                std::cerr << "C=" << cs.C << std::endl;
                return;
            }
            
            
            //__________________________________________________________________
            //
            // Most generic case: keep the altered topology
            //__________________________________________________________________
            std::cerr << "P="   << P   << std::endl;
            std::cerr << "Lam=" << Lam << std::endl;
            
            matrix_t  P2(dof,dof);
            mkl::mul_rtrn(P2, P, P);
            if( !LU.build(P2 ))
                throw exception("singular chemical constraints/rank");
            
            const size_t na = M-fix;
            matrix_t A(na,M);
            matrix_t B;
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
                    B.make(fix,M);
                    for(size_t i=1;i<=fix;++i)
                    {
                        B[i][indices[i]] = 1;
                    }
                }
            }
            std::cerr << "A=" << A << std::endl;
            std::cerr << "B=" << B << std::endl;
            
            
            //__________________________________________________________________
            //
            // compute Xstar
            //__________________________________________________________________
            vector_t Xstar(M,0.0);
            {
                vector_t U(dof,0.0);
                mkl::set(U,Lam);
                LU.solve(P2, U);
                mkl::mul_trn(Xstar, P, U);
            }
            std::cerr << "Xstar=" << Xstar << std::endl;
            
            
            //__________________________________________________________________
            //
            // compute Q, an orthogonal complementary of P
            // with nv = M-dof
            //__________________________________________________________________
            const size_t nq = M-dof;
            matrix_t Q(nq,M);
            {
                matrix_t F(M,M);
                for(size_t i=1;i<=dof;++i)
                {
                    for(size_t j=1;j<=M;++j)
                    {
                        F[j][i] = P[i][j];
                    }
                }
                //std::cerr << "F=" << F << std::endl;
                vector_t __W(M,0);
                matrix_t __V(M,M);
                if( ! math::svd<double>::build(F, __W, __V) )
                    throw exception("singular chemical constraint/SVD");
                //std::cerr << "F=" << F << std::endl;
                for(size_t i=1;i<=nq;++i)
                {
                    for(size_t j=1;j<=M;++j)
                    {
                        Q[i][j] = F[j][i+dof];
                    }
                }
            }
            std::cerr << "Q=" << Q << std::endl;
            
            //__________________________________________________________________
            //
            // Compute the generator of the variables space
            //__________________________________________________________________
            {
                const size_t nv = P.rows + B.rows;
                matrix_t F(M,nv);
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
                std::cerr << "F=" << F << std::endl;
            }
            
            
        }
        
        
    }
}
