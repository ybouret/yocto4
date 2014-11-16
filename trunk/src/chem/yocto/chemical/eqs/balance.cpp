#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"

namespace yocto
{
    namespace chemical
    {

        using namespace math;
        
        
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
            tao::set(C,C0);
            
            if(count_negative())
            {
                
            }
            
            
            
            
            return true;
        }
        

    }

}