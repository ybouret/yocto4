#include "yocto/math/alg/spike.hpp"


namespace yocto
{
    namespace math
    {
        spike:: spike( size_t pos ) throw() :
        position(pos),
        lower(pos),
        upper(pos)
        {

        }

        spike:: ~spike() throw()
        {
        }


    }
}
