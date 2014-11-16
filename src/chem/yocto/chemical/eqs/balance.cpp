#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"

namespace yocto
{
    namespace chemical
    {

        using namespace math;
        
        
        void equilibria:: compute_limits() throw()
        {
            assert(M>0);
            for(size_t i=N;i>0;--i)
            {
                eqs[i]->compute_limits(C);
            }
        }
        
        size_t equilibria:: count_negative() throw()
        {
            size_t ans=0;
            for(size_t i=M;i>0;--i)
            {
                if(active[i])
                {
                    const double Ci = C[i];
                    if(Ci<0)
                    {
                        ++ans;
                        Cneg[i] = Ci;
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
            std::cerr << "C=" << C << std::endl;
            
            if(count_negative())
            {
                compute_limits();
            }
            
            
            
            
            return true;
        }
        

    }

}