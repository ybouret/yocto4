#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
     
        
        equilibria:: equilibria() throw() :
        M(0),
        N(0)
        {
        }
        
        equilibria:: ~equilibria() throw()
        {
            
        }
        
        equilibrium &equilibria:: add( equilibrium *pEq )
        {
            assert(pEq);
            equilibrium::pointer eq(pEq);
            for(size_t i=1;i<=N;++i)
            {
                if(eqs[i]->name == eq->name)
                {
                    throw exception("equilibria(multiple '%s')", eq->name.c_str());
                }
            }
            eqs.push_back(eq);
            ++(size_t&)N;
            return *eq;
        }
        
        void equilibria:: remove(const string &name)
        {
            for(size_t i=1;i<=N;++i)
            {
                if(name==eqs[i]->name)
                {
                    bswap(eqs[i],eqs[N]);
                    eqs.pop_back();
                    --(size_t&)N;
                    return;
                }
            }
        }
        

        
        void equilibria:: clear() throw()
        {
            (size_t &)M = 0;
            
            Phi.    release();
            Nu.     release();
            xi.     release();
            Gamma.  release();
            K.      release();
            
            active. release();
        }
   
        void equilibria:: compile_for( const library &lib )
        {
            clear();
            (size_t &)M = lib.size();
            assert(N==eqs.size());
            
            try
            {
                if(M<=0)
                    throw exception("equilibria: no species");
                
                active.make(M,false);
                
                if(N>0)
                {
                    K.     make(N,0.0);
                    Gamma. make(N,0.0);
                    xi.    make(N,0.0);
                    Nu.    make(N,M);
                    Phi.   make(N,M);
                }
                
                for(size_t i=1;i<=N;++i)
                {
                    eqs[i]->initialize(Nu[i],active);
                }
                
            }
            catch(...)
            {
                clear();
                throw;
            }
        }

    
    }
    
}
