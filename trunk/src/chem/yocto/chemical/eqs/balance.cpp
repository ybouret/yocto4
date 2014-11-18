#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        using namespace math;
        
        
        
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
            
            Ineg.free();
            
            for(size_t i=1;i<=M;++i)
            {
                flag[i] = false;
                if(active[i])
                {
                    const double Ci = C[i];
                    if(Ci<0)
                    {
                        ++ans;
                        Ineg.push_back(i);
                        flag[i] = true;
                    }
                }
            }
            assert(Ineg.size()==ans);
            return ans;
        }
        
        double equilibria:: computeE( array<ptrdiff_t> &dEdC )
        {
            assert(M==dEdC.size());
            Ineg.free();
            double ans = 0;
            for(size_t i=1;i<=M;++i)
            {
                dEdC[i] = 0;
                flag[i] = false;
                if(active[i])
                {
                    const double Ci = C[i];
                    if(Ci<0)
                    {
                        Ineg.push_back(i);
                        ans -= Ci;
                        dEdC[i] = 1;
                        flag[i] = true;
                    }
                }
                
            }
            return 0.5*ans;
        }
        
        
        bool equilibria:: balance( array<double> &C0 )
        {
            assert(M>0);
            assert(C0.size()>=M);
            if(N<=0)
                return true;
            
            tao::set(C,C0);
            std::cerr << "balanceC=" << C << std::endl;
            
            ivector_t  dEdC(M,0); //!< dE/dC is a flag...
            ivector_t  beta(N,0); //!< minimize dir
            ivector_t  incr(M,0); //!< the final increment
            vector_t   alpha(M,as_capacity);
            
            double E = 0;
            while( (E=computeE(dEdC)) > 0 )
            {
                const size_t Q =Ineg.size(); assert(Q>0);
                tao::mul(beta,Nu,dEdC);
                std::cerr << "E=" << E << std::endl;
                std::cerr << "dEdC=" << dEdC << std::endl;
                std::cerr << "beta0=" << beta << std::endl;
                
                online.free();
                for(size_t i=1;i<=N;++i)
                {
                    equilibrium::pointer &pEq = eqs[i];
                    bool                  isOn = false;
                    for(size_t j=Q;j>0;--j)
                    {
                        if(pEq->involves(Ineg[j]))
                        {
                            // the equilibrium MAY be online
                            pEq->compute_limits(C);
                            pEq->show_limits(std::cerr);
                            isOn = ! pEq->blocked;
                            break;
                        }
                    }
                    if(isOn)
                    {
                        std::cerr << "\t" << pEq->name << " is ONLINE" << std::endl;
                        online.push_back(pEq);
                    }
                    else
                    {
                        std::cerr << "\t" << pEq->name << " is OFFLINE or BLOCKED" << std::endl;
                        beta[i] = 0;
                    }
                }
                const size_t R=online.size();
                if(R<=0)
                    throw exception("no d.o.f. to balance equilibria");
                
                std::cerr << "beta1=" << beta << std::endl;
                
                tao::mul_trn(incr,Nu,beta);
                
                std::cerr << "incr=" << incr << std::endl;
                
                alpha.free();
                for(size_t j=1;j<=Q;++j)
                {
                    const size_t    jj    = Ineg[j];
                    const double    Cj    = C[j];
                    const ptrdiff_t Dj    = incr[jj];
                    if(Dj)
                    {
                        const double    Aj    = -Cj/Dj;
                        std::cerr << "C[" << jj << "]=" << Cj << ", Dj=" << Dj << " --> "  << Aj << std::endl;
                    }
                }
                
                break;
            }
            
            
            
            
            return true;
        }
        
        
    }
    
}