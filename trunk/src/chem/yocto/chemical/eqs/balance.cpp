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
            vector_t   aindx(M,as_capacity);
            
            double E     = 0;
            size_t count = 0;
            while( (E=computeE(dEdC)) > 0 )
            {
                ++count;
                std::cerr << "==== #" << count << std::endl;
                
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
                {
                    std::cerr << "-- No D.O.F to Balance..." << std::endl;
                    return false;
                }
                
                
                std::cerr << "beta1=" << beta << std::endl;
                
                tao::mul_trn(incr,Nu,beta);
                
                std::cerr << "incr=" << incr << std::endl;
                
                alpha.free();
                aindx.free();
                
                for(size_t j=1;j<=M;++j)
                {
                    if(active[j])
                    {
                        const double Cj = C[j];
                        const double Dj = incr[j];
                        if(Cj<0)
                        {
                            if(Dj<0)
                            {
                                throw exception("equilibria.balance: arithmetic failure!!!");
                            }
                            if(Dj>0)
                            {
                                alpha.push_back( (-Cj)/Dj );
                                aindx.push_back(j);
                            }
                        }
                        else
                        {
                            assert(Cj>=0);
                            if(Dj<0)
                            {
                                alpha.push_back( Cj/(-Dj) );
                                aindx.push_back(j);
                            }
                        }
                    }
                }
                if(alpha.size()<=0)
                {
                    std::cerr << "-- No possible increase" << std::endl;
                    return false;
                }
                co_qsort(alpha, aindx);
                std::cerr << "aindx=" << aindx << std::endl;
                std::cerr << "alpha=" << alpha << std::endl;
                const double fac = alpha[1];
                
                // compute with carefull addition
                for(size_t j=1;j<=M;++j)
                {
                    dC[j] = 0;
                    if(active[j])
                    {
                        const ptrdiff_t scale = incr[j];
                        const double    Cj    = C[j]; // save old value
                        const double    delta = (dC[j]=scale*fac);
                        C[j] += delta;          // compute new value
                        if(scale>0)
                        {
                            if(Cj<=0&&C[j]>=0) C[j] = 0;
                        }
                        else
                        {
                            if(scale<0)
                            {
                                if(Cj>=0&&C[j]<=0) C[j] = 0;
                            }
                            // else do nothing
                        }
                    }
                    
                }
                
                C[aindx[1]] = 0;
                std::cerr << "dC=" << dC << std::endl;
                std::cerr << "C="  << C  << std::endl;
                
                
            }
            
            
            
            
            return true;
        }
        
        
    }
    
}