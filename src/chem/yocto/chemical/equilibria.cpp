#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
     
        
        equilibria:: equilibria() throw() :
        equilibrium::database(),
        M(0),
        N(0)
        {
        }
        
        equilibria:: ~equilibria() throw()
        {
            
        }
        
        
        void equilibria:: clear() throw()
        {
            (size_t &)M = 0;
            (size_t &)N = 0;
            
            Nu0.release();
            Nu.release();
            xi.release();
            Gamma.release();
            K.release();
        }
   
        void equilibria:: compile_for( const library &lib )
        {
            (size_t &)M = lib.size();
            (size_t &)N = this->size();
            try
            {
                if(M<=0)
                    throw exception("equilibria: no species");
                
                
                if(N>0)
                {
                    K.     make(N,0);
                    Gamma. make(N,0);
                    xi.    make(N,0);
                    Nu.    make(N,M);
                    Nu0.   make(N,M);
                }
                
                iterator eq = begin();
                for(size_t i=1;i<=N;++i,++eq)
                {
                    (*eq)->initialize(Nu[i]);
                }
                Nu0.assign(Nu);
                
            }
            catch(...)
            {
                clear();
                throw;
            }
        }

    
    }
    
}
