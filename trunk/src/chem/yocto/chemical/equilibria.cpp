#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/code/ipower.hpp"

namespace yocto
{
    namespace chemical
    {
     
        using namespace math;
        
        equilibria:: equilibria() throw() :
        M(0),
        N(0),
        optH(this, & equilibria::computeH),
        optG(this, & equilibria::computeG)
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
            Delta = 0;
            
            U.      release();
            xis.    release();
            Q.      release();
            xip.    release();
            online. release();
            W.      release();
            Phi.    release();
            Nu.     release();
            xi.     release();
            Gamma.  release();
            K.      release();
            
            fixedC. release();
            fixedJ. release();
            aboot.  release();
            Xstar.  release();
            aindx.  release();
            alpha.  release();
            dCp.    release();
            beta.   release();
            
            Jneg.   release();
            Cs.     release();
            active. release();
            dC.     release();
            C.      release();
        }
   
        void equilibria:: compile_for( const library &lib )
        {
            clear();
            (size_t &)M = lib.size();
            
            try
            {
                assert(N==eqs.size());
                for(size_t i=1;i<=N;++i)
                {
                    eqs[i]->validate();
                }

                if(M<=0)
                    throw exception("equilibria: no species");
                
                C.      make(M,0);
                dC.     make(M,0);
                active. make(M,0);
                Cs.     make(M,0);
                Jneg.   make(M,0);
                beta.   make(M,0);
                dCp.    make(M,0);
                alpha.  make(M,0);
                aindx.  make(M,0);
                Xstar.  make(M,0);
                aboot.  make(M,0);
                fixedJ. make(M,0);
                fixedC. make(M,0);
                
                if(N>0)
                {
                    K.      make(N,0.0);
                    Gamma.  make(N,0.0);
                    xi.     make(N,0.0);
                    Nu.     make(N,M);
                    Phi.    make(N,M);
                    W.      make(N,N);
                    online. ensure(N);
                    xip.    make(N,0);
                    Q.      make(N,M);
                    xis.    make(N,0);
                    U.      make(N,0);
                }
                
                restore_topology();
                
                
                
            }
            catch(...)
            {
                clear();
                throw;
            }
        }
        
        void equilibria:: restore_topology()
        {
            for(size_t i=N;i>0;--i)
            {
                eqs[i]->initialize(Nu[i],active);
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

        bool equilibria:: GammaHasConverged() const
        {
            for(size_t i=N;i>0;--i)
            {
                //if(false == eqs[i]->cvg) return false;
            }
            //return true;
            return false;
        }

    
    }
    
}
