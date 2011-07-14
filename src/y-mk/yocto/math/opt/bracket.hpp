/** \file bracket.hpp
 */

#ifndef YOCTO_MATH_OPT_BRACKET_INCLUDED
#define YOCTO_MATH_OPT_BRACKET_INCLUDED 1

#include "yocto/math/triplet.hpp"

namespace yocto {
	
	namespace math {
		
		
		template <class T>
		struct bracket
		{
			//! bracketing a minimum to call minimize
			/**
			 search in x.a->x.c, assume that f.a and f.c are computed
			 */
			static bool inside( typename numeric<T>::function &func, triplet<T> &x, triplet<T> &f );
			
		};
		
		
		
	}
	
}

#endif
