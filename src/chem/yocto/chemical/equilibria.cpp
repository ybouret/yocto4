#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/tao.hpp"

namespace yocto
{
    namespace chemical
    {
     
        using namespace math;
        
        equilibria:: equilibria() throw() :
        M(0),
        N(0)
        {
        }
        
        equilibria:: ~equilibria() throw()
        {
            
        }
        
        
        void equilibria:: output(std::ostream &os) const
        {
            static const size_t ns = 64;
            os << '/'; for(size_t i=0;i<ns;++i) os << '-'; os << std::endl;
            for(size_t i=1;i<=N;++i)
            {
                os << '|' << ' ' << *eqs[i] << std::endl;
            }
            os << '\\';for(size_t i=0;i<ns;++i) os << '-';
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
            (size_t&)(eq->indx) = N;
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
                    for(size_t j=1;j<=N;++j)
                    {
                        (size_t &)(eqs[j]->indx) = j;
                    }
                    return;
                }
            }
        }
        

        
        void equilibria:: clear() throw()
        {
            (size_t &)M = 0;
            
            online. release();
            Phi.    release();
            Nu.     release();
            xi.     release();
            Gamma.  release();
            K.      release();
            
            Jneg.   release();
            active. release();
            dC.     release();
            C.      release();
        }
   
        void equilibria:: compile_for( const library &lib )
        {
            clear();
            (size_t &)M = lib.size();
            assert(N==eqs.size());
            
            for(size_t i=1;i<=N;++i)
            {
                eqs[i]->validate();
            }
            
            try
            {
                if(M<=0)
                    throw exception("equilibria: no species");
                
                C.      make(M,0.0);
                dC.     make(M,0.0);
                active. make(M,0.0);
                Jneg.   make(M,0);
                
                
                if(N>0)
                {
                    K.      make(N,0.0);
                    Gamma.  make(N,0.0);
                    xi.     make(N,0.0);
                    Nu.     make(N,M);
                    Phi.    make(N,M);
                    online. ensure(N);
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
        
        equilibrium       & equilibria:: operator[](size_t i) throw()
        {
            assert(i>=1);assert(i<=eqs.size());assert(N==eqs.size());
            return *eqs[i];
        }
        const equilibrium & equilibria:: operator[](size_t i) const throw()
        {
            assert(i>=1);assert(i<=eqs.size());assert(N==eqs.size());
            return *eqs[i];
        }



    
    }
    
}
