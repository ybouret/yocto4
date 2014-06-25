#include "yocto/chemical/equilibria.hpp"

namespace yocto
{
    namespace chemical
    {
        equilibria:: ~equilibria() throw()
        {
        }
        
        equilibria:: equilibria() :
        M(0),
        N(0)
        {
        }
        
        void equilibria:: cleanup() throw()
        {
            (size_t &)M = 0;
            (size_t &)M = 0;
            
            Phi.release();
            Gamma.release();
            K.release();
            NuP.release();
            NuR.release();
            Nu.release();
        }
        
        
        void equilibria:: startup(const collection &lib)
        {
            cleanup();
            
            (size_t &)M = lib.size();
            (size_t &)N = this->size();
            
            try
            {
                if(N>0)
                {
                    Nu.make(N,M);
                    NuR.make(N,M);
                    K.make(N,0.0);
                    Gamma.make(N,0.0);
                    Phi.make(N,M);
                    
                    //__________________________________________________________
                    //
                    // fill constant parts
                    //__________________________________________________________
                    size_t i = 0;
                    for( iterator k=begin();k!=end();++k)
                    {
                        const equilibrium &eq = **k;
                        ++i;
                        eq.fill(Nu[i], NuR[i], NuP[i]);
                    }
                    
                }
                
            }
            catch(...)
            {
                cleanup();
                throw;
            }
            
        }
        
        void equilibria:: output( std::ostream &os ) const
        {
            static const size_t nsep = 64;
            for(size_t i=0;i<nsep;++i) os << '#'; os << std::endl;
            for(const_iterator i=begin();i!=end();++i)
            {
                const equilibrium &eq = **i;
                os << "# " << eq << std::endl;
            }
            for(size_t i=0;i<nsep;++i) os << '#';

        }

        
    }
}
