#include "yocto/math/alg/spike.hpp"


namespace yocto
{
    namespace math
    {
        spike:: spike( size_t pos, double val ) throw() :
        position(pos),
        lower(pos),
        upper(pos),
        value(val)
        {

        }

        spike:: ~spike() throw()
        {
        }


    }
}
