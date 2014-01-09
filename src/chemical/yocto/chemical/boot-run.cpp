#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/sort/quick.hpp"

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
                cs.restore_topology();
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
                throw exception("singular chemical constraints");
            
            vector_t Mu(dof,0.0);       // temporary to compute linear constraints
            vector_t dX(M,0.0);         // to detect concentration updates
            vector_t sigma(M,0.0);      // the virtual source term
            
            ios::ocstream fp("err.dat",false);
            
            static const size_t MIN_ITER_PER_SPECIES = 2;
            const  size_t       min_iter = min_of<size_t>(2,M * MIN_ITER_PER_SPECIES);
            
            double oldRMS = -1;
            for(size_t iter=1;;++iter)
            {
                std::cerr << std::endl << "iter=" << iter << std::endl;
                std::cerr << "C0=" << cs.C << std::endl;
                //--------------------------------------------------------------
                // virtual source term
                //--------------------------------------------------------------
                mkl::mul(Mu, P, cs.C);
                mkl::subp(Mu, Lam);
                LU.solve(P2, Mu);
                mkl::mul_trn(cs.dC, P, Mu);
                std::cerr << "sigma=" << cs.dC << std::endl;
                std::cerr << "fixed=" << cs.fixed << std::endl;
                for(size_t i=M;i>0;--i)
                {
                    if(cs.fixed[i]) cs.dC[i] = 0;
                }
                mkl::set(sigma,cs.dC); // save it
                //--------------------------------------------------------------
                // legalize the source term
                //--------------------------------------------------------------
                cs.legalize_dC(t, false);
                std::cerr << "dC2=" << cs.dC << std::endl;
                
                //--------------------------------------------------------------
                // save C
                //--------------------------------------------------------------
                mkl::set(dX,cs.C);
                
                //--------------------------------------------------------------
                // update C
                //--------------------------------------------------------------
                mkl::add(cs.C, cs.dC);
                cs.normalize_C(t);
                
                std::cerr << "C1=" << cs.C << std::endl;
                
                //--------------------------------------------------------------
                // compute effective displacement
                //--------------------------------------------------------------
                mkl::subp(dX,cs.C);
                std::cerr << "dX=" << dX << std::endl;
                const double RMS = mkl::rms(dX);
                if(RMS>0)
                {
                    fp("%g %g\n", double(iter), log10(RMS));
                }
                
                //--------------------------------------------------------------
                // test convergence on dX
                //--------------------------------------------------------------
                if(iter>min_iter)
                {
                    assert(oldRMS>=0);
                    if(RMS>=oldRMS)
                        break;
                }
                oldRMS = RMS;
            }
            
            //__________________________________________________________________
            //
            // Test residual
            //__________________________________________________________________
            std::cerr << "Converged for dX" << std::endl;
            std::cerr << "sigma=" << sigma << std::endl;
            std::cerr << "dX   =" << dX << std::endl;
            
            //------------------------------------------------------------------
            // normalize the P/Lambda system
            //------------------------------------------------------------------
            for(size_t i=dof;i>0;--i)
            {
                const double fac = mkl::norm_L2(P[i]);
                for(size_t j=M;j>0;--j)
                {
                    P[i][j] /= fac;
                }
                Lam[i] /= fac;
            }
            
            //------------------------------------------------------------------
            // compute the RMS on the constraints
            //------------------------------------------------------------------
            mkl::mul(Mu, P, cs.C);
            mkl::sub(Mu, Lam);
            const double rms = mkl::rms(Mu);
            
            std::cerr << "rms=" << rms << std::endl;
            
        }
        
        
    }
}
