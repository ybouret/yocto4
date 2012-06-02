#include "yocto/aqueous/chemsys.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/ipower.hpp"

namespace yocto 
{
    namespace aqueous
    {
        
        chemsys:: ~chemsys() throw()
        {
        }
        
        chemsys:: chemsys( const library &L ) throw() :
        equilibria(),
        nu(),
        nuR(),
        nuP(),
        K(),
        Gamma(),
	C(),
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
        
        equilibrium & chemsys:: create( const string &name, double K )
        {
            equilibrium::ptr eq( new equilibrium(lib,name,K) );
            
            if( ! insert( eq ) )
            {
                throw exception("chemsys(multiple equilibrium [%s])", name.c_str()); 
            }
            
            return *eq;
        }
        
        
        void chemsys:: build() 
        {
            const size_t N = this->size();
            const size_t M = lib.size();
                        
            if( N > 0 && M > 0 )
            {
                
                //! memory allocation
                nu.make(N,M);
                nuR.make(N,M);
                nuP.make(N,M);
                K.make(N,0.0);
                Gamma.make(N,0.0);
                
                //! compute topology
                equilibria::iterator p = begin();
                for( size_t i=1; i <= N; ++i, ++p )
                {
                    const equilibrium &eq = **p;
                    eq.fill(nu[i], nuR[i], nuP[i] );
                }
            }
            C.make(M,0.0);
            
        }
        
        void chemsys:: computeGamma( const solution &s, double t)
        {
            const size_t N = this->size();
            const size_t M = lib.size();
#if !defined(NDEBUG)
            
#endif
            s.put(C);
            equilibria::iterator p = begin();
            for( size_t i=1; i <= N; ++i, ++p )
            {
                double lhs = (K[i] = (**p).K(t));
                double rhs = 1;
                const array<ptrdiff_t> &Ri = nuR[i];
                const array<ptrdiff_t> &Pi = nuP[i];
                for( size_t j=1; j <= M; ++j )
                {
                    const int Rij = Ri[j]; assert(Rij>=0);
                    if(Rij>0)
                        lhs *= ipower(C[j],Rij);
                    const int Pij = Pi[j]; assert(Pij>=0);
                    if(Pij>0)
                        rhs *= ipower(C[j],Pij);
                }
                Gamma[i] = lhs - rhs;
            }
        }
        
        
    }
    
}
