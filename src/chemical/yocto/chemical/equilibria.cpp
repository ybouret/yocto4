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
        tiny( numeric<double>::tiny ),
        C(),
        dC(),
        nuR(),
        nuP(),
        nu(),
        Gamma(),
        dtGam(),
        Phi(),
        W(),
        xi(),
        LU(),
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
        
        //! build a random concentration after scaling
        void equilibria:: trial( urand32 &ran, double t)
        {
            for(size_t i=C.size();i>0;--i) C[i] = 0;
            
            for( iterator eq=begin();eq!=end();++eq)
            {
                (**eq).append(C,ran);
            }
            
            normalize_C(t);
            for(size_t i=C.size();i>0;--i) C[i] = max_of<double>(C[i],0);

            
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
            LU.release();
            xi.release();
            W.release();
            Phi.release();
            dtGam.release();
            Gamma.release();
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
                    nuR.make(N,M);
                    nuP.make(N,M);
                    nu.make(N,M);
                    Gamma.make(N,0);
                    dtGam.make(N,0);
                    Phi.make(N,M);
                    W.make(N,N);
                    xi.make(N,0);
                    LU.ensure(N);
                    
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
                    
                    std::cerr << "nu=" << nu << std::endl;
                    
                }
            }
            catch(...)
            {
                reset();
                throw;
            }
        }
        
        void equilibria:: compute_Gamma_and_Phi( double t, bool compute_derivatives)
        {
            const size_t N = nu.rows;
            const size_t M = nu.cols;
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
                dtGam[i]  = 0;
                if(compute_derivatives)
                {
                    dtGam[i] = dervs( Eq.K, t, time_scale ) * lhs;
                }
            }
            
            for(size_t i=N;i>0;--i)
            {
                if(Fabs(Gamma[i])<=tiny) Gamma[i] = 0;
            }
            
        }
        
        
        void equilibria:: compute_Gamma_and_W( double t, bool compute_derivatives)
        {
            compute_Gamma_and_Phi(t, compute_derivatives);
            mkl::mul_rtrn(W, Phi, nu);
            if( !LU.build(W) )
                throw exception("equilibria: invalid composition");
        }
        
        void equilibria:: normalize_C( double t )
        {
            const size_t N = nu.rows;
            if(N>0)
            {
                
                
                const size_t M = nu.cols;
                for(size_t i=M;i>0;--i)
                {
                    C[i] = max_of<double>(0,C[i]);
                }
                
            NEWTON_STEP:
                compute_Gamma_and_W(t,false);
                mkl::set(xi,Gamma);
                LU.solve(W, xi);
                mkl::mul_trn(dC, nu, xi);
                mkl::sub(C, dC);
                //std::cerr.flush();
                //std::cerr << "// Newton tiny=" << tiny << "/" << numeric<double>::tiny << std::endl;
                //std::cerr << "C=" << C << std::endl;
                //std::cerr << "//iter=" << iter << std::endl;
                //std::cerr << "Gamma=" << Gamma << std::endl;
                //std::cerr << "dC=" << dC << std::endl;
                
                bool converged = true;
                for(size_t i=M;i>0;--i)
                {
                    if( Fabs(C[i]) <= tiny ) C[i] = 0;
                    double err = Fabs(dC[i]);
                    if( err <= tiny ) err = 0;
                    if( err > Fabs( ftol * C[i] ) )
                    {
                        converged = false;
                    }
                }
                if(!converged) goto NEWTON_STEP;
            }
        }
        
        
        void equilibria:: legalize_dC( double t, bool computeDerivatives )
        {
            const size_t N = nu.rows;
            if(N>0)
            {
                compute_Gamma_and_W(t,computeDerivatives);
                mkl::muladd(dtGam, Phi, dC);
                LU.solve(W,dtGam);
                mkl::mulsub_trn(dC, nu, dtGam);
                for(size_t i=dC.size();i>0;--i)
                {
                    if( fabs(dC[i]) <= tiny ) dC[i] = 0;
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
        
        
        
    }
    
    
}

