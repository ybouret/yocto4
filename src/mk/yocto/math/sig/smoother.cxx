#include "yocto/math/ztype.hpp"
#include "yocto/math/sig/smoother.hpp"

namespace yocto
{
    namespace math
    {
        template <>
        smoother<real_t>:: ~smoother() throw()
        {
        }
        
        template <>
        smoother<real_t>:: smoother()
        {
            
        }
        
    }
}
