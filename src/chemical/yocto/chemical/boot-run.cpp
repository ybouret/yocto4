#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/ios/ocstream.hpp"


namespace yocto
{
    namespace chemical
    {
        
        
        typedef math::algebra<double> mkl;
        
        
        static inline void generate_starting(equilibria          &cs,
                                             const array<bool>   &fixed,
                                             const array<double> &X0,
                                             urand32             &ran,
                                             const double        &t)
        {
            assert(cs.C.size()==fixed.size());
            assert(X0.size()==fixed.size());
            cs.trial(ran,t);
            for(size_t i=X0.size();i>0;--i)
            {
                if(fixed[i]) cs.C[i] = X0[i];
            }
        }
        
        
        void boot::loader::operator()(equilibria &cs, collection &lib, double t)
        {
            static const double FTOL = math::numeric<double>::ftol;
            
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
            vector<bool> fixed(M,false);
            vector_t     X0(M,0.0);
            
            const size_t dof   = dispatch(P, Lam, fixed, X0);
            const size_t fix   = Nc-dof;
            std::cerr << "#DOF="  << dof << std::endl;
            std::cerr << "#FIX="  << fix << std::endl;
            std::cerr << "fixed=" << fixed << std::endl;
            std::cerr << "X0   =" << X0    << std::endl;
            
            
            //__________________________________________________________________
            //
            // Biased stoichiometry
            //__________________________________________________________________
            matrix_t Nu = cs.nu;
            assert(Nu.rows==N);
            assert(Nu.cols==M);
            for(size_t i=1;i<=N;++i)
            {
                for(size_t j=1;j<=M;++j)
                {
                    if(fixed[j]) Nu[i][j] = 0;
                }
            }
            std::cerr << "nu=" << cs.nu << std::endl;
            std::cerr << "Nu=" << Nu << std::endl;
            
            //__________________________________________________________________
            //
            // Create a starting point
            //__________________________________________________________________
            
            //------------------------------------------------------------------
            // create a random valid concentration
            //------------------------------------------------------------------
            generate_starting(cs, fixed, X0, ran, t);
            std::cerr << "C0=" << cs.C << std::endl;
            
            //------------------------------------------------------------------
            // biased equilibrium
            //------------------------------------------------------------------
            cs.normalize_with(Nu,t);
            std::cerr << "C1="  << cs.C << std::endl;
            
            //__________________________________________________________________
            //
            // No DOF: just a biased chemistry
            //__________________________________________________________________
            if(dof<=0)
            {
                cs.normalize_C(t);
                std::cerr << "C=" << cs.C << std::endl;
                return;
            }
            
            
            //__________________________________________________________________
            //
            // Most generic case
            //__________________________________________________________________
            std::cerr << "P=" << P << std::endl;
            std::cerr << "Lam=" << Lam << std::endl;
            
            matrix_t P2(dof,dof);
            mkl::mul_rtrn(P2, P, P);
            if( !LU.build(P2 ))
                throw exception("singular chemical constraints");
            
            vector_t Mu(dof,0.0);
            vector_t dX(M,0.0);
            vector_t sigma(M,0.0);
            for(size_t iter=1;;++iter)
            {
                std::cerr << std::endl << " iter=" << iter << std::endl;
                //--------------------------------------------------------------
                // virtual source term
                //--------------------------------------------------------------
                mkl::mul(Mu, P, cs.C);
                mkl::subp(Mu, Lam);
                LU.solve(P2, Mu);
                mkl::mul_trn(cs.dC, P, Mu);
                std::cerr << "sigma=" << cs.dC << std::endl;
                for(size_t i=M;i>0;--i)
                {
                    if(fixed[i]) cs.dC[i] = 0; // should be true
                }
                mkl::set(sigma,cs.dC);
                //--------------------------------------------------------------
                // legalize the source term
                //--------------------------------------------------------------
                cs.legalize_with(Nu, t, false);
                std::cerr << "dC2=" << cs.dC << std::endl;
                
                //--------------------------------------------------------------
                // save C
                //--------------------------------------------------------------
                mkl::set(dX,cs.C);
                
                //--------------------------------------------------------------
                // update C
                //--------------------------------------------------------------
                mkl::add(cs.C, cs.dC);
                cs.normalize_with(Nu,t);
                
                std::cerr << "C=" << cs.C << std::endl;
                
                //--------------------------------------------------------------
                // compute effective displacement
                //--------------------------------------------------------------
                mkl::sub(dX,cs.C);
                std::cerr << "dX=" << dX << std::endl;
                
                //--------------------------------------------------------------
                // test convergence on dX
                //--------------------------------------------------------------
                bool converged = true;
                for(size_t i=M;i>0;--i)
                {
                    double err = fabs(dX[i]);
                    if(err<=cs.tiny) err = 0;
                    if( err > FTOL * fabs(cs.C[i]))
                    {
                        converged = false;
                        break;
                    }
                    dX[i] = err;
                }
                if(converged)
                    break;
            }
            std::cerr << "Converged for dX" << std::endl;
            
            
        }
        
        
    }
}
