#include "yocto/aqueous/chemsys.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace aqueous
    {
        
        typedef algebra<double> mkl;
        typedef lu<double>      LU;
        
        chemsys:: ~chemsys() throw()
        {
        }
        
        chemsys:: chemsys( const library &L, const double frac_tol ) throw() :
        equilibria(),
        ftol( Fabs(frac_tol) ),
        nu(),
        nuR(),
        nuP(),
        K(),
        Gamma(),
        dtGam(),
        C(),
        Phi(),
        W(),
        LU(),
        xi(),
        dC(),
        drvs(),
        t_scale(1e-4),
        lib(L)
        {
        }
        
        equilibrium & chemsys:: create( const string &name, const equilibrium::constant &K )
        {
            equilibrium::ptr eq( new equilibrium(lib,name,K) );
            
            if( ! insert( eq ) )
            {
                throw exception("chemsys(multiple equilibrium [%s])", name.c_str());
            }
            
            return *eq;
        }
        
        
        equilibrium & chemsys:: create( const char *id, const equilibrium::constant &K )
        {
            const string name(id);
            return create(name,K);
        }
        
        
        equilibrium & chemsys:: create( const string &name, double K )
        {
            equilibrium::ptr eq( new equilibrium(lib,name,K) );
            
            if( ! insert( eq ) )
            {
                throw exception("chemsys(multiple equilibrium [%s])", name.c_str());
            }
            
            return *eq;
        }
        
        equilibrium & chemsys:: create( const char *id, double K )
        {
            const string name(id);
            return create(name,K);
        }
        
        
        void chemsys:: build()
        {
            const size_t N = this->size();
            const size_t M = lib.size();
            
            if( N > 0 && M > 0 )
            {
                
                //--------------------------------------------------------------
                // memory allocation
                //--------------------------------------------------------------
                nu.make(N,M);
                nuR.make(N,M);
                nuP.make(N,M);
                K.make(N,0.0);
                Gamma.make(N,0.0);
                dtGam.make(N,0.0);
                Phi.make(N,M);
                W.make(N,N);
                LU.ensure(N);
                xi.make(N,0.0);
                dC.make(M,0.0);
                
                //--------------------------------------------------------------
                // compute topology
                //--------------------------------------------------------------
                equilibria::iterator p = begin();
                for( size_t i=1; i <= N; ++i, ++p )
                {
                    const equilibrium &eq = **p;
                    eq.fill(nu[i], nuR[i], nuP[i] );
                }
            }
            C.make(M,0.0);
        }
        
        
        void chemsys:: computeGammaAndPhi(double t, bool computeDerivative)
        {
            const size_t N = this->size();
            const size_t M = lib.size();
            
            //------------------------------------------------------------------
            // simultaneous Gamma/Phi
            //------------------------------------------------------------------
            equilibria::iterator p = begin();
            for( size_t i=1; i <= N; ++i, ++p )
            {
                equilibrium::constant   Fcn = (**p).K;
                const double            Ki  = (K[i] = Fcn(t));
                double                  lhs = 1;
                double                  rhs = 1;
                const array<ptrdiff_t> &Ri  = nuR[i];
                const array<ptrdiff_t> &Pi  = nuP[i];
                
                for( size_t j=1; j <= M; ++j )
                {
                    const int    Rij  = Ri[j]; assert(Rij>=0);
                    const double Cj   = C[j];
                    double       PhiR = 0;
                    double       PhiP = 0;
                    if(Rij>0)
                    {
                        //------------------------------------------------------
                        // Gamma Term
                        //------------------------------------------------------
                        const double Cpm1 = ipower(Cj,Rij-1);
                        lhs *= Cpm1 * Cj;
                        
                        //------------------------------------------------------
                        // Phi Term
                        //------------------------------------------------------
                        PhiR = Ki * Rij * Cpm1;
                        for( size_t k=1; k <= M; ++k )
                        {
                            if( j != k )
                            {
                                const int Rik = Ri[k];
                                if( Rik > 0 )
                                    PhiR *= ipower( C[k], Rik );
                            }
                        }
                    }
                    
                    const int Pij = Pi[j]; assert(Pij>=0);
                    if(Pij>0)
                    {
                        //------------------------------------------------------
                        // Gamma Term
                        //------------------------------------------------------
                        const double Cpm1 = ipower(Cj, Pij-1);
                        rhs *= Cpm1 * Cj;
                        
                        //------------------------------------------------------
                        // Phi Term
                        //------------------------------------------------------
                        PhiP = Pij * Cpm1;
                        for( size_t k=1; k <= M; ++k )
                        {
                            if( j != k )
                            {
                                const int Pik = Pi[k];
                                if( Pik > 0 )
                                    PhiP *= ipower( C[k], Pik );
                            }
                        }
                        
                    }
                    Phi[i][j] = PhiR - PhiP;
                }
                
                Gamma[i] = Ki * lhs - rhs;
                
                if( computeDerivative )
                    dtGam[i] = lhs * drvs( Fcn, t, t_scale );
                else
                    dtGam[i] = 0;
                
            }
            
        }
        
        void chemsys:: computeW(double t, bool computeDerivative )
        {
            computeGammaAndPhi(t,computeDerivative);
            //------------------------------------------------------------------
            // compute W = Phi * trn(nu)
            //------------------------------------------------------------------
            mkl::mul_rtrn(W, Phi, nu);
            
            //------------------------------------------------------------------
            // prepare W
            //------------------------------------------------------------------
            if( !LU.build(W) )
                throw exception("Singular composition!");
            
        }
        
        void chemsys:: normalize( double t)
        {
            const size_t N = this->size();
            const size_t M = lib.size();
            
            if( N > 0 )
            {
            NEWTON_STEP:
                //==============================================================
                // compute inverse jacobian
                //==============================================================
                computeW(t,false);
                
                //==============================================================
                // compute extent
                //==============================================================
                for( size_t i=N;i>0;--i) xi[i] = -Gamma[i];
                LU.solve(W,xi);
                
                //==============================================================
                // compute dC
                //==============================================================
                mkl::mul_trn(dC, nu, xi);
                
                //==============================================================
                // update and check convergence
                //==============================================================
                bool converged = true;
                for( size_t j=M;j>0;--j)
                {
                    const double dC_j = dC[j];
                    const double C_j  = (C[j]+=dC_j);
                    if( Fabs(dC_j) > Fabs( ftol * C_j ) )
                        converged = false;
                }
                
                if( !converged )
                    goto NEWTON_STEP;
            }
            
            for( size_t j=M;j>0;--j)
                C[j] = max_of<double>(0.0,C[j]);
            
        }
        
        void chemsys:: reduce(double t)
        {
            const size_t N = this->size();
            
            if( N > 0 )
            {
                //--------------------------------------------------------------
                // compute inverse jacobian for C, and dtGam
                //--------------------------------------------------------------
                computeW(t,true);
                
                //--------------------------------------------------------------
                // dtGam += Phi * dC
                //--------------------------------------------------------------
                mkl::muladd(dtGam, Phi,dC);
                
                //--------------------------------------------------------------
                // xi = inv(Phi*nu') * ( dtGam + Phi * dC ), in place
                //--------------------------------------------------------------
                LU.solve(W,dtGam);
                mkl::mulsub_trn(dC, nu, dtGam);
                
            }
            
        }
        
    }
    
}
