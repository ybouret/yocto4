#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/ipower.hpp"

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
        N(0),
        Nu(),
        Nu0(),
        dNu(),
        dNuP(),
        K(),
        Gamma(),
        Phi(),
        W(),
        xi(),
        limits(),
        online(),
        scaled(),
        gammaC(),
        active(),
        dC(),
        bad(),
        drvs(),
        h(1e-4),
        callback(this,& equilibria::odecb )
        {
        }
        
        
        void equilibria:: odecb(array<double> &Y, double t)
        {
            if(!normalize(t,Y,true))
                throw exception("ODE: unable to normalize trial concentrations");
        }
        
        double equilibria:: derivate( func_type &f, const double t )
        {
            return drvs(f,t,h);
        }
        
        void equilibria:: cleanup() throw()
        {
            (size_t &)M = 0;
            (size_t &)M = 0;
            
            bad.release();
            dC.release();
            active.release();
            
            gammaC.release();
            scaled.release();
            online.release();
            limits.release();
            xi.release();
            W.release();
            Phi.release();
            Gamma.release();
            K.release();
            dNuP.release();
            dNu.release();
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
                    dNu.make(N,0);
                    dNuP.make(N,0);
                    K.make(N,0.0);
                    Gamma.make(N,0.0);
                    Phi.make(N,M);
                    W.make(N,N);
                    xi.make(N,0.0);
                    {
                        const extent ex0;
                        limits.make(N,ex0);
                    }
                    online.ensure(N);
                    scaled.make(N,0.0);
                    gammaC.make(N,0.0);
                    //__________________________________________________________
                    //
                    // fill constant parts
                    //__________________________________________________________
                    size_t i = 0;
                    for( iterator k=begin();k!=end();++k)
                    {
                        equilibrium &eq = **k;
                        ++i;
                        dNuP[i] = eq.compile(Nu[i],lib);
                        dNu[i]  = eq.DeltaNu;
                    }
                    //std::cerr << "Nu=" << Nu << std::endl;
                    //std::cerr << "dNu=" << dNu << std::endl;
                    //std::cerr << "dNuP=" << dNuP << std::endl;
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
                    if( !lu_t::build(W) )
                        throw exception("equilibria: invalid rank");
                    
                }
                active.make(M,false);
                dC.make(M,0.0);
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
        
        void equilibria:: computeK(double t)
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                K[i] = eq.K(t);
            }
            
        }
        
        void equilibria:: compute_scaled_concentrations() throw()
        {
            for(size_t i=N;i>0;--i)
            {
                const double KK = K[i];
                const int    dd = dNu[i];
                if(dd!=0)
                {
                    scaled[i] = pow(KK,1.0/dd);
                }
                else
                {
                    scaled[i] = 1.0;
                }
                gammaC[i] = ipower(scaled[i],size_t(dNuP[i]));
            }
            
        }
        
        void equilibria:: generate(array<double> &C, alea_t &ran) throw()
        {
            assert(C.size()>=M);
            for(size_t j=M;j>0;--j)
            {
                if(active[j]>0)
                {
                    C[j] = 0.0;
                }
            }
            
            for(size_t i=N;i>0;--i)
            {
                const double cc = scaled[i] * ran();
                for(size_t j=M;j>0;--j)
                {
                    if(active[j]>0)
                    {
                        C[j] +=  cc;
                    }
                }
            }
            
            
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
        
        void equilibria:: computeGammaAndPhi(double t, const array<double> &C)
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                Gamma[i] = eq.computeGammaAndPhi(Phi[i],t,C,K[i]);
            }
            cleanPhi();
        }
        
        void equilibria:: updateGammaAndPhi(const array<double> &C) throw()
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                Gamma[i] = eq.updateGammaAndPhi(Phi[i],C,K[i]);
            }
            cleanPhi();
        }
        
        
        void equilibria:: cleanPhi() throw()
        {
            for(size_t j=M;j>0;--j)
            {
                if(active[j]<=0)
                {
                    for(size_t i=N;i>0;--i)
                    {
                        Phi[i][j] = 0;
                    }
                }
            }
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
            cleanPhi();
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
            //std::cerr << "active=" << active << std::endl;
        }
        
        void equilibria:: restore_topology() throw()
        {
            Nu.assign(Nu0);
            find_active_species();
        }
        
        
        
    }
}
