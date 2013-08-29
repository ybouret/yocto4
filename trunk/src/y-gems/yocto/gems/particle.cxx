#include "yocto/math/ztype.hpp"
#include "yocto/gems/particle.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace gems
    {
        using namespace math;
        
        template <>
        particle<real_t>:: ~particle() throw() {}
        
        template <>
        particle<real_t>:: particle() throw() :
        r(), w( numeric<real_t>::one ),
        v(), m( numeric<real_t>::one ),
        a(),
        uuid(0),
        type(0)
        {
        }
        
        
    }
}