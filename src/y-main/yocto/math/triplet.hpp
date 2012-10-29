/** \file triplet.hpp
*/

#ifndef _YOCTO_TRIPLET_HPP_
#define _YOCTO_TRIPLET_HPP_

#include "yocto/math/types.hpp"
#include <iosfwd>

namespace yocto {

	namespace math {

		template <class T>
		struct triplet {
			T a,b,c;

			void to_scilab( std::ostream &os ) const;

			friend
			inline std::ostream & operator<<( std::ostream &os, const triplet &x)
			{
				x.to_scilab(os);
				return os;
			}
			
			T       &operator[]( size_t index ) throw();
			const T &operator[]( size_t index ) const throw();
            bool is_ordered() const throw();
		};

	}
}
#endif
