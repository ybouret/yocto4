#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"

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
            dNu2 = 0;
            
            ANu2.   release();
            Nu2.    release();
            Phi.    release();
            Nu.     release();
            xi.     release();
            Gamma.  release();
            K.      release();
            
            Cneg.   release();
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
                active. make(M,false);
                Cneg.   make(M,0.0);
               
                
                if(N>0)
                {
                    K.     make(N,0.0);
                    Gamma. make(N,0.0);
                    xi.    make(N,0.0);
                    Nu.    make(N,M);
                    Phi.   make(N,M);
                    Nu2.   make(N,N);
                    ANu2.  make(N,N);
                    iNu2.  make(N,M);
                }
                
                for(size_t i=1;i<=N;++i)
                {
                    eqs[i]->initialize(Nu[i],active);
                    eqs[i]->online = true;
                }
                
                if(N>0)
                {
                    tao::mmul_rtrn(Nu2, Nu, Nu);
                    dNu2 = determinant_of(Nu2);
                    if(dNu2<=0)
                        throw exception("equilibria: singular system");
                    adjoint(ANu2, Nu2);
                    tao::mmul(iNu2,ANu2,Nu);
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
