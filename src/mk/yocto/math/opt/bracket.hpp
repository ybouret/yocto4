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
             may stop on sides...
			 */
			static void inside( typename numeric<T>::function &func, triplet<T> &x, triplet<T> &f );
			
            //! bracketting a minimum to call minimize
            /**
             search from xa,xb, assuming that fa and fb are computed.
             */
            static void expand( typename numeric<T>::function &func, triplet<T> &x, triplet<T> &f );
		};
		
		
		
	}
	
}

#endif
