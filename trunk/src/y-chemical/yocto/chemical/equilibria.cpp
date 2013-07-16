#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include <iostream>
#include "yocto/code/ipower.hpp"

namespace yocto
{
    
    namespace chemical
    {
        typedef math::algebra<double> mkl;
        
        equilibria:: ~equilibria() throw() {}
        
        equilibria:: equilibria() :
        equilibrium::db(),
        pressure(standard_pressure),
        temperature(standard_temperature),
        nuR(),
        nuP()
        {
            
        }
        
        
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
        
        void equilibria:: reset() throw()
        {
            C.release();
            W.release();
            Phi.release();
            dGam.release();
            Gamma.release();
            nu.release();
            nuP.release();
            nuR.release();
        }
        
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
        
        void equilibria:: build_from( collection &lib )
        {
            reset();
            lib.update_indices();
            try
            {
                const size_t N = this->size();
                if(N>0)
                {
                    const size_t M = lib.size();
                    
                    //----------------------------------------------------------
                    // allocate memory
                    //----------------------------------------------------------
                    nuR.make(N,M);
                    nuP.make(N,M);
                    nu.make(N,M);
                    Gamma.make(N,0);
                    dGam.make(N,0);
                    Phi.make(N,M);
                    W.make(N,N);
                    C.make(M,0);
                    
                    //----------------------------------------------------------
                    // compute topological parts
                    //----------------------------------------------------------
                    size_t i=1;
                    for( iterator eq=begin();eq!=end();++eq,++i)
                    {
                        (**eq).fill(nuR[i], nuP[i]);
                    }
                    std::cerr << "nuR=" << nuR << std::endl;
                    std::cerr << "nuP=" << nuP << std::endl;
                    
                    for(i=1;i<=N;++i)
                    {
                        for(size_t j=1; j<=M;++j)
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
        
        void equilibria:: compute_Gamma_and_W( double t, bool compute_derivatives)
        {
            const size_t N = nu.rows;
            const size_t M = nu.cols;
            iterator     eq = begin();
            for(size_t i=1;i<=N;++i,++eq)
            {
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
                Gamma[i] = (*eq)->K(t, pressure, temperature) * lhs - rhs;
                dGam[i]  = 0;
                if(compute_derivatives)
                {
                    dGam[i] = 0;
                }
            }
            std::cerr << "Gamma=" << Gamma << std::endl;
        }
        
        
    }
    
    
}

