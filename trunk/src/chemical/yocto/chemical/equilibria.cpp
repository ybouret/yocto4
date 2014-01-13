#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include <iostream>
#include "yocto/code/ipower.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    using namespace math;
    
    namespace chemical
    {
        typedef math::algebra<double> mkl;
        
        
        equilibria:: ~equilibria() throw() {}
        
        
        //======================================================================
        // initialize
        //======================================================================
        equilibria:: equilibria() :
        equilibrium::db(),
        pressure(standard_pressure),
        temperature(standard_temperature),
        ftol( numeric<double>::ftol ),
        time_scale(1e-4),
        tiny(  numeric<double>::tiny ),
        sqrt_tiny( numeric<double>::sqrt_tiny ),
        C(),
        dC(),
        nuR(),
        nuP(),
        nu(),
        Nu(),
        fixed(),
        Gamma(),
        dtGam(),
        Phi(),
        W(),
        xi(),
        LU(),
        CC(),
        dervs()
        {
        }
        
        void equilibria:: scale_all( double t ) const throw()
        {
            for( const_iterator eq=begin();eq!=end();++eq)
            {
                (**eq).scale(t);
            }
        }
        
        void equilibria:: cleanup_C() throw()
        {
            for(size_t i=C.size();i>0;--i)
            {
                if(C[i]<=tiny) C[i] = 0;
                assert(C[i]>=0);
            }
        }
        
        
        //! build a random concentration after scaling
        void equilibria:: trial( urand32 &ran, double t)
        {
            
            for(size_t i=dC.size();i>0;--i)
            {
                dC[i] = 0;
            }
            
            for( iterator eq=begin();eq!=end();++eq)
            {
                (**eq).append(dC,ran);
            }
            
            for(size_t i=dC.size();i>0;--i)
            {
                if( !fixed[i] ) C[i] = dC[i];
            }
            
            normalize_C(t);
            
        }
        
        
        //======================================================================
        // append a constant equilibrium
        //======================================================================
        equilibrium & equilibria:: add( const string &name, const double K)
        {
            equilibrium::ptr p( new constant_equilibrium(name,K) );
            
            if( !insert(p) )
                throw exception("chemical.equilibria.insert(multiple '%s')", name.c_str());
            
            return *p;
        }
        
        
        equilibrium & equilibria:: add( const char   *name, const double K)
        {
            const string NAME(name);
            return add(NAME,K);
        }
        
        //======================================================================
        // release all memory
        //======================================================================
        void equilibria:: reset() throw()
        {
            CC.release();
            LU.release();
            xi.release();
            W.release();
            Phi.release();
            dtGam.release();
            Gamma.release();
            fixed.release();
            Nu.release();
            nu.release();
            nuP.release();
            nuR.release();
            dC.release();
            C.release();
        }
        
        //======================================================================
        // I/O
        //======================================================================
        std::ostream & operator<<( std::ostream &os, const equilibria &eqs)
        {
            static const size_t nch = 64;
            for(size_t i=0;i<nch;++i) os << '#'; os << std::endl;
            for( equilibria::const_iterator i=eqs.begin();i!=eqs.end();++i)
            {
                const equilibrium &eq = **i;
                os << '#' << ' '  << eq << std::endl;
            }
            for(size_t i=0;i<nch;++i) os << '#';
            return os;
        }
        
        //======================================================================
        // acquire all required memory
        //======================================================================
        void equilibria:: build_from( collection &lib )
        {
            reset();
            lib.update_indices();
            try
            {
                const size_t N = this->size();
                const size_t M = lib.size();
                C.make(M,0);
                dC.make(M,0);
                
                if(N>0)
                {
                    
                    //----------------------------------------------------------
                    // allocate memory
                    //----------------------------------------------------------
                    nuR.   make(N,M);
                    nuP.   make(N,M);
                    nu.    make(N,M);
                    Nu.    make(N,M);
                    fixed. make(M,false);
                    Gamma. make(N,0);
                    dtGam. make(N,0);
                    Phi.   make(N,M);
                    W.     make(N,N);
                    xi.    make(N,0);
                    LU.    ensure(N);
                    CC.    make(M,0.0);
                    
                    //----------------------------------------------------------
                    // compute topological parts
                    //----------------------------------------------------------
                    size_t i=1;
                    for( iterator eq=begin();eq!=end();++eq,++i)
                    {
                        const equilibrium &Eq = **eq;
                        if(!Eq.is_valid())
                            throw exception("invalid equilibrium <%s>", Eq.name.c_str());
                        Eq.fill(nuR[i], nuP[i]);
                    }
                    
                    for(i=N;i>0;--i)
                    {
                        for(size_t j=M;j>0;--j)
                        {
                            nu[i][j] = nuP[i][j] - nuR[i][j];
                        }
                    }
                    Nu.assign(nu);
                    
                }
            }
            catch(...)
            {
                reset();
                throw;
            }
        }
        
        //======================================================================
        // normalized RMS for Gamma
        //======================================================================
        void equilibria:: compute_Gamma(const double t)  throw()
        {
            const size_t N = nu.rows;
            if(N<=0)
                return;
            const size_t    M  = nu.cols;
            iterator        eq = begin();
            for(size_t i=1;i<=N;++i,++eq)
            {
                equilibrium &Eq = **eq;
                const double Ki =  Eq.K(t);
                double lhs = 1;
                double rhs = 1;
                for(size_t j=M;j>0;--j)
                {
                    {
                        const size_t r = nuR[i][j];
                        if(r>0)
                        {
                            lhs *= ipower(C[j],r);
                        }
                    }
                    
                    {
                        const size_t p = nuP[i][j];
                        if(p>0)
                        {
                            rhs *= ipower(C[j],p);
                        }
                    }
                    
                }
                Gamma[i] = Ki*lhs -rhs;
                //if(Fabs(Gamma[i])<=tiny) Gamma[i] = 0;
            }
            
        }
        
        //======================================================================
        // normalized RMS for Gamma
        //======================================================================
        double equilibria:: Gamma2RMS() const throw()
        {
            assert(Gamma.size()>0);
            const size_t   N   = Gamma.size();
            double         ans = 0;
            const_iterator eq  = begin();
            for(size_t i=1;i<=N;++i,++eq)
            {
                const equilibrium &Eq = **eq;
                double             G  = fabs(Gamma[i]);
                const size_t DeltaNuP = Eq.NuP;
                if(DeltaNuP>0)
                    G = pow(G,1.0/DeltaNuP);
                ans += G*G;
            }
            return sqrt(ans/N);
        }
        
        //======================================================================
        // compute Gamma return scaled rms
        //======================================================================
        double equilibria:: compute_rms(double t) throw()
        {
            compute_Gamma(t);
            return Gamma2RMS();
        }
        
        
        
        //======================================================================
        // compute Gamma and Phi=dGamma/dC
        //======================================================================
        void equilibria:: compute_Gamma_and_Phi( double t, bool compute_derivatives)
        {
            const size_t N  = nu.rows;
            const size_t M  = nu.cols;
            iterator     eq = begin();
            Phi.ldz();
            for(size_t i=1;i<=N;++i,++eq)
            {
                equilibrium &Eq = **eq;
                const double Ki =  Eq.K(t);
                double lhs = 1;
                double rhs = 1;
                for(size_t j=M;j>0;--j)
                {
                    {
                        const size_t r = nuR[i][j];
                        if(r>0)
                        {
                            lhs *= ipower(C[j],r);
                            double tmp = Ki * r * ipower(C[j],r-1);
                            for(size_t k=M;k>j;--k)
                            {
                                tmp *= ipower(C[k],nuR[i][k]);
                            }
                            for(size_t k=j-1;k>0;--k)
                            {
                                tmp *= ipower(C[k],nuR[i][k]);
                            }
                            Phi[i][j] = tmp;
                        }
                    }
                    
                    {
                        const size_t p = nuP[i][j];
                        if(p>0)
                        {
                            rhs *= ipower(C[j],p);
                            double tmp = p * ipower(C[j],p-1);
                            for(size_t k=M;k>j;--k)
                            {
                                tmp *= ipower(C[k],nuP[i][k]);
                            }
                            for(size_t k=j-1;k>0;--k)
                            {
                                tmp *= ipower(C[k],nuP[i][k]);
                            }
                            Phi[i][j] -= tmp;
                        }
                    }
                    
                }
                Gamma[i] = Ki * lhs - rhs;
                dtGam[i] = 0;
                if(compute_derivatives)
                {
                    dtGam[i] = dervs( Eq.K, t, time_scale ) * lhs;
                }
                //if(Fabs(Gamma[i])<=tiny) Gamma[i] = 0;
            }
            
            
            for(size_t j=M;j>0;--j)
            {
                if(fixed[j])
                {
                    for(size_t i=N;i>0;--i)
                    {
                        Phi[i][j] = 0;
                    }
                }
            }
            
            
        }
        
        
        void equilibria:: update_topology() throw()
        {
            Nu.assign(nu);
            for(size_t j=fixed.size();j>0;--j)
            {
                if(fixed[j])
                {
                    for(size_t i=Nu.rows;i>0;--i) Nu[i][j] = 0;
                }
            }
        }
        
        
        //! activate all
        void equilibria:: restore_topology() throw()
        {
            for(size_t j=fixed.size();j>0;--j)
            {
                fixed[j] = false;
            }
            Nu.assign(nu);
        }
        
        
        bool equilibria:: compute_Gamma_and_W( double t, bool compute_derivatives)
        {
            compute_Gamma_and_Phi(t, compute_derivatives);
            mkl::mul_rtrn(W, Phi,Nu);
            return LU.build(W);
        }
        
        
               
        
        void equilibria:: legalize_dC( double t, bool computeDerivatives)
        {
            
            const size_t N = nu.rows;
            if(N>0)
            {
                compute_Gamma_and_W(t,computeDerivatives);
                mkl::muladd(dtGam, Phi, dC);
                LU.solve(W,dtGam);
                mkl::mulsub_trn(dC, Nu, dtGam);
                for(size_t i=dC.size();i>0;--i)
                {
                    if( fixed[i] || (fabs(dC[i]) <= sqrt_tiny) )
                        dC[i] = 0;
                }
            }
            
        }
        
        
        
        
        void equilibria:: load_C( const array<double> &y ) throw()
        {
            assert(C.size()<=y.size());
            for(size_t i=C.size();i>0;--i) C[i] = y[i];
        }
        
        void equilibria:: save_C( array<double> &y ) const throw()
        {
            assert(C.size()<=y.size());
            for(size_t i=C.size();i>0;--i) y[i] = C[i];
        }
        
        
        void equilibria:: compute_Chi(matrix_t &Chi, double t)
        {
            const size_t M = C.size();
            Chi.make(M,M);
            const size_t N = nu.rows;
            if(N>0)
            {
                compute_Gamma_and_W(t,false);
                matrix_t tmpPhi(Phi);
                LU.solve(W, tmpPhi);
                mkl::mul_ltrn(Chi, nu, tmpPhi);
            }
        }
        
        
    }
    
    
}

