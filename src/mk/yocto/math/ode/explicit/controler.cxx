#include "yocto/math/ztype.hpp"
#include "yocto/math/ode/explicit/controler.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            
            template <>
            explicit_controler<real_t>:: ~explicit_controler() throw() {}
            
            template <>
            explicit_controler<real_t>:: explicit_controler( size_t na ) :
            Field<real_t>::Arrays(na)
            {
            }
            
        }
    }
}
