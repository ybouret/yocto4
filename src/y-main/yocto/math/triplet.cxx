#include "yocto/math/triplet.hpp"
#include "yocto/math/ztype.hpp"

#include <iostream>

namespace yocto {

	namespace math
	{


		template <>
		real_t & triplet<real_t>:: operator[]( size_t index ) throw()
		{
			assert( index >= 1 );
			assert( index <= 3 );
			return *(((&a)-1)+index);
		}

		template <>
		const real_t & triplet<real_t>:: operator[]( size_t index ) const throw()
		{
			assert( index >= 1 );
			assert( index <= 3 );
			return *(((&a)-1)+index);
		}


		template <>
		void triplet<real_t>::to_scilab( std::ostream &os ) const
		{
			os << "[ " << a << " " << b << " " << c << " ]'";
		}

        template <>
        bool triplet<real_t>:: is_ordered() const throw()
        {
            return (a <= b && b <= c) || (a >= b && b >= c);
        }

        
	}

}
