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
                if(active[i])
                {
                    const double Ci = C[i];
                    if(Ci<0)
                    {
                        ++ans;
                        Ineg.push_back(i);
                    }
                }
            }
            assert(Ineg.size()==ans);
            return ans;
        }
        
        
        
        bool equilibria:: balance( array<double> &C0 )
        {
            assert(M>0);
            assert(C0.size()>=M);
            
            tao::set(C,C0);
            std::cerr << "balanceC=" << C << std::endl;
            
            size_t  Mb=0;
            
            
            while( (Mb=count_negative()) > 0 )
            {
                //______________________________________________________________
                //
                // detected negative conc among active
                //______________________________________________________________
                std::cerr << "Ineg=" << Ineg << std::endl;
                const size_t Q = Ineg.size();
                
                //______________________________________________________________
                //
                // find online equilibria
                //______________________________________________________________
                online.free();
                for(size_t i=1;i<=N;++i)
                {
                    equilibrium::pointer &pEq = eqs[i];
                    for(size_t j=Mb;j>0;--j)
                    {
                        if(pEq->involves( Ineg[j] ) )
                        {
                            online.push_back(pEq);
                            pEq->compute_limits(C);
                            online[i]->show_limits(std::cerr);
                            break;
                        }
                    }
                }
                const size_t R = online.size();
                assert(R>0);
                
                imatrix_t Beta(Q,R);
                for(size_t i=1;i<=R;++i)
                {
                    const size_t ii = online[i]->indx;
                    for(size_t j=1;j<=Q;++j)
                    {
                        const size_t jj = Ineg[j];
                        Beta[j][i] = Nu[ii][jj];
                        
                    }
                }
                std::cerr << "Beta=" << Beta << std::endl;
                
                
                break;
            }
            
            
            
            
            
            return true;
        }
        
        
    }
    
}