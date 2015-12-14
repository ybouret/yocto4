#include "yocto/math/triplet.hpp"
#include "yocto/math/ztype.hpp"

#include <iostream>

namespace yocto {

	namespace math
	{




        template <>
        bool triplet<real_t>:: is_ordered() const throw()
        {
            return (a <= b && b <= c) || (a >= b && b >= c);
        }

        
	}

}
