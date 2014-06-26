#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"

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
            Nu.release();
        }
        
        
        void equilibria:: startup(const collection &lib)
        {
            cleanup();
           
            for( iterator k=begin();k!=end();++k)
            {
                const equilibrium &eq = **k;
                if(!eq.is_valid())
                    throw exception("Invalid Equilibrium '%s'", eq.name.c_str());
            }

            
            (size_t &)M = lib.size();
            (size_t &)N = this->size();
            
            try
            {
                if(N>0)
                {
                    Nu.make(N,M);
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
                        equilibrium &eq = **k;
                        ++i;
                        eq.compile(Nu[i],lib);
                    }
                    std::cerr << "Nu=" << Nu << std::endl;
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

        
        void equilibria:: computeGamma(double t, const array<double> &C )
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                Gamma[i] = eq.computeGamma(t, C, K[i]);
            }
        }
        
        void equilibria:: updateGamma(const array<double> &C)
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                Gamma[i] = eq.updateGamma(C,K[i]);
            }
            
        }

        
    }
}
