#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"

namespace yocto
{
    namespace chemical
    {
        
        using namespace math;
        
        
        void equilibria:: enforce_limits() throw()
        {
            assert(xi.size()==N);
            for(size_t i=N;i>0;--i)
            {
                double &xx = xi[i];
                const equilibrium &eq = *eqs[i];
                if(eq.online)
                {
                    //std::cerr << "checking " << xx << " for " << eq.name << std::endl;
                    if(xx>=0)
                    {
                        if(eq.forward.blocked)
                        {
                            xx = 0;
                        }
                        else
                        {
                            if(eq.forward.limited && xx>=eq.forward.maximum)
                            {
                                xx = eq.forward.maximum;
                            }
                        }
                    }
                    else
                    {
                        assert(xx<0);
                        if(eq.reverse.blocked)
                        {
                            xx = 0;
                        }
                        else
                        {
                            if(eq.reverse.limited && (-xx)>=eq.reverse.maximum)
                            {
                                xx = -eq.reverse.maximum;
                            }
                        }
                    }
                }
                else
                {
                    xx = 0;
                }
            }
        }
        
        void equilibria:: compute_limits() throw()
        {
            assert(M>0);
            assert(N==eqs.size());
            for(size_t i=N;i>0;--i)
            {
                eqs[i]->compute_limits(C);
                eqs[i]->show_limits(std::cerr);
            }
        }
        
        size_t equilibria:: count_negative() throw()
        {
            size_t ans=0;
            assert(M==C.size());
            
            for(size_t j=N;j>0;--j)
            {
                eqs[j]->online = false;
            }
            
            for(size_t i=M;i>0;--i)
            {
                if(active[i])
                {
                    const double Ci = C[i];
                    if(Ci<0)
                    {
                        ++ans;
                        Cneg[i] = -Ci;
                        // check if the species #i is involved in eq #j
                        for(size_t j=N;j>0;--j)
                        {
                            eqs[j]->check_online_for(i);
                        }
                    }
                }
            }
            return ans;
        }
        
        
        bool equilibria:: balance( array<double> &C0 )
        {
            assert(M>0);
            assert(C0.size()>=M);
            
            tao::set(C,C0);
            std::cerr << "balanceC=" << C << std::endl;
            
            if(count_negative()>0)
            {
                //______________________________________________________________
                //
                // how can we move
                //______________________________________________________________
                compute_limits();
                std::cerr << "Cneg=" << Cneg << std::endl;
#if 0
                for(size_t j=N;j>0;--j)
                {
                    std::cerr << eqs[j]->name << " : " << ( eqs[j]->online ? "ON" : "OFF") << std::endl;
                }
#endif
                //______________________________________________________________
                //
                // compute xi
                //______________________________________________________________
                tao::mul(xi, iNu2, Cneg);
                for(size_t i=N;i>0;--i) xi[i] /= dNu2;
                std::cerr << "xi0=" << xi << std::endl;
                
                //______________________________________________________________
                //
                // check xi
                //______________________________________________________________
                enforce_limits();
                std::cerr << "xi1=" << xi << std::endl;
                
                
                //______________________________________________________________
                //
                // compute dC
                //______________________________________________________________
                
                tao::mul_trn(dC, Nu, xi);
                std::cerr << "dC  =" << dC << std::endl;
                std::cerr << "oldC=" << C << std::endl;
                //______________________________________________________________
                //
                // append dC carefully
                //______________________________________________________________
                for(size_t i=M;i>0;--i)
                {
                    if(active[i])
                    {
                        const double Ci   = C[i];
                        const double dCi  = dC[i];
                        const double newC = Ci+dCi;
                        if(Ci>=0)
                        {
                            C[i] = max_of<double>(0.0,newC);
                        }
                        else
                        {
                            C[i] = newC;
                        }
                    }
                    else
                    {
                        assert(Fabs(dC[i])<=0);
                    }
                }
                std::cerr << "C=" << C << std::endl;
                
            }
            
            
            
            
            return true;
        }
        
        
    }
    
}