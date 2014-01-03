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
            explicit_controler<real_t>:: explicit_controler( size_t num_arrays ) :
            lw_arrays<real_t,ode::memory_type>(num_arrays)
            {
            }
            
        }
    }
}
