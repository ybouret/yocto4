#include "yocto/math/kernel/woodbury.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace math
    {
        
        
        template <>
        bool woodbury<z_type>:: build(matrix<z_type>       &B,
                                      const lu<z_type>     &LU,
                                      const matrix<z_type> &A,
                                      const matrix<z_type> &U,
                                      const matrix<z_type> &V)
        {
            
            return false;
        }
        
    }
    
    
}
