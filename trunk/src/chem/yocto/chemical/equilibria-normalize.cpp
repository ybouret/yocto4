#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace chemical
    {
        typedef math::algebra<double> mkl;
        
        bool  equilibria:: normalize( double t, array<double> &C )
        {
            
            if(N<=0)
                return true;
            
            return false;
        }
        
    }
}
