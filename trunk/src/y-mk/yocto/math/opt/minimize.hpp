/** \file minimize.hpp
*/

#ifndef _YOCTO_MINIMIZE_HPP_
#define _YOCTO_MINIMIZE_HPP_ 1


#include "yocto/math/triplet.hpp"

namespace yocto {

	namespace math {


		template <typename T>
		struct minimize
		{
			T ftol; //!< fractional tolerance, internaly >= sqrt(epsilon)

			//! (x.a <= x.b <= x.c) || (x.a>=x.b>=x.c) and f.b <= f.a and f.b <= f.c
			void operator()( typename numeric<T>::function &func, triplet<T> &x, triplet<T> &f);

			

		};

	}

}

#endif
