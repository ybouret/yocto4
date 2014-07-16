#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"

namespace yocto
{
    namespace chemical
    {
        typedef math::algebra<double> mkl;
        
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
            
            bad.release();
            Ctmp.release();
            dC.release();
            active.release();
            
            online.release();
            LU.release();
            limits.release();
            xi.release();
            W.release();
            Phi.release();
            Gamma.release();
            K.release();
            Nu0.release();
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
                    Nu0.make(N,M);
                    K.make(N,0.0);
                    Gamma.make(N,0.0);
                    Phi.make(N,M);
                    W.make(N,N);
                    xi.make(N,0.0);
                    const extent ex0;
                    limits.make(N,ex0);
                    LU.make(N,0.0);
                    online.ensure(N);
                    
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
                    
                    //__________________________________________________________
                    //
                    // copy the full topology
                    //__________________________________________________________
                    Nu0.assign(Nu);
                    
                    //__________________________________________________________
                    //
                    // Topology must have a maximal rank
                    //__________________________________________________________
                    mkl::mul_rtrn(W, Nu, Nu);
                    if( !LU.build(W) )
                        throw exception("equilibria: invalid rank");
                    
                }
                active.make(M,false);
                dC.make(M,0.0);
                Ctmp.make(M,0.0);
                bad.ensure(M);
                find_active_species();
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
        
        
        double equilibria:: computeGamma(double t, const array<double> &C )
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                Gamma[i] = eq.computeGamma(t, C, K[i]);
            }
            return getF();
        }
        
        double equilibria:: updateGamma(const array<double> &C)
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                Gamma[i] = eq.updateGamma(C,K[i]);
            }
            return getF();
        }
        
        double equilibria:: computeGammaAndPhi(double t, const array<double> &C)
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                Gamma[i] = eq.computeGammaAndPhi(Phi[i],t,C,K[i]);
            }
            return getF();
        }
        
        double equilibria:: updateGammaAndPhi(const array<double> &C) throw()
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                Gamma[i] = eq.updateGammaAndPhi(Phi[i],C,K[i]);
            }
            return getF();
        }
        
        
        void equilibria:: updatePhi(const array<double> &C) throw()
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                eq.updatePhi(Phi[i], C, K[i]);
            }
        }
        
        
        
        
        double equilibria:: getF() const throw()
        {
            double ans = 0;
            for(size_t i=N;i>0;--i)
            {
                const double g = Gamma[i];
                ans += g*g;
            }
            return 0.5 * ans;
        }
        
        
        
        void  equilibria:: find_active_species() throw()
        {
            for(size_t j=M;j>0;--j)
            {
                active[j] = 0;
            }
            
            for(size_t i=N;i>0;--i)
            {
                const array<double> &nu = Nu[i];
                for(size_t j=M;j>0;--j)
                {
                    if(int(nu[j])!=0)
                    {
                        ++active[j];
                    }
                }
            }
            std::cerr << "active=" << active << std::endl;
        }
        

        
    }
}
