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
        
        double equilibria:: computeE( array<ptrdiff_t> &dEdC )
        {
            assert(M==dEdC.size());
            Jneg.free();
            double ans = 0;
            for(size_t i=1;i<=M;++i)
            {
                dEdC[i] = 0;
                if(active[i])
                {
                    const double Ci = C[i];
                    if(Ci<0)
                    {
                        Jneg.push_back(i);
                        ans -= Ci;
                        dEdC[i] = 1;
                    }
                }
                
            }
            return ans;
        }
        
        
        bool equilibria:: balance( array<double> &C0 )
        {
            assert(M>0);
            assert(C0.size()>=M);
            if(N<=0)
                return true;
            
            //__________________________________________________________________
            //
            // load the concentration
            //__________________________________________________________________
            tao::set(C,C0);
            
            double E     = 0;
            size_t count = 0;
            
            //__________________________________________________________________
            //
            // compute the Energy and the descent direction in beta
            //__________________________________________________________________
            while( (E=computeE(beta)) > 0 )
            {
                ++count;
                if(count>M)
                {
                    std::cerr << "-- Too Many Iterations in Equilibria.Balance" << std::endl;
                    return false;
                }
                
                
                //______________________________________________________________
                //
                // compute the integer descent extern xip = Nu*beta
                //______________________________________________________________
                tao::mul(xip,Nu,beta);
                
                //______________________________________________________________
                //
                // analyse equilibria to remove blocked ones
                //______________________________________________________________
                online.free();
                const size_t Q =Jneg.size(); assert(Q>0);
                for(size_t i=N;i>0;--i)
                {
                    equilibrium::pointer &pEq = eqs[i];
                    bool                  isOn = false;
                    for(size_t j=Q;j>0;--j)
                    {
                        if(pEq->involves(Jneg[j]))
                        {
                            //__________________________________________________
                            //
                            // the equilibrium MAY be online
                            //__________________________________________________
                            pEq->compute_limits(C);
                            //pEq->show_limits(std::cerr);
                            isOn = ! pEq->blocked;
                            break;
                        }
                    }
                    
                    if(isOn)
                    {
                        //std::cerr << "\t" << pEq->name << " is ONLINE" << std::endl;
                        online.push_back(pEq);
                    }
                    else
                    {
                        //std::cerr << "\t" << pEq->name << " is OFFLINE or BLOCKED" << std::endl;
                        xip[i] = 0;
                    }
                }
                const size_t R=online.size();
                if(R<=0)
                {
                    std::cerr << "-- No D.O.F to Balance..." << std::endl;
                    return false;
                }
                
                //______________________________________________________________
                //
                // compute the integer concentration decrease direction:
                // dCp = Nu' * xip
                //______________________________________________________________
                tao::mul_trn(dCp,Nu,xip);
                
                
                //______________________________________________________________
                //
                // Find how far we go: first zeroed conc
                //______________________________________________________________
                alpha.free();
                aindx.free();
                
                for(size_t j=1;j<=M;++j)
                {
                    if(active[j])
                    {
                        const double Cj = C[j];
                        const double Dj = dCp[j];
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

                //______________________________________________________________
                //
                // compute with careful updates
                //______________________________________________________________
                const double fac = alpha[1];
                for(size_t j=M;j>0;--j)
                {
                    dC[j] = 0;
                    if(active[j])
                    {
                        const ptrdiff_t scale = dCp[j];
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
                            else
                            {
                                assert(0==scale);
                            }
                        }
                    }
                    
                }
                
                C[aindx[1]] = 0;
                //std::cerr << "C" << count << "=" << C << std::endl;
            }
            
            copy_to(C0);
            return true;
        }
        
        
    }
    
}

