/** \file color.hpp
*/

#ifndef _YOCTO_COLOR_HPP_
#define _YOCTO_COLOR_HPP_ 1

#include "yocto/math/types.hpp"
#include "yocto/bitwise.hpp"

namespace yocto {

	namespace color {

		template <typename T>
		struct rgba {
			T r,g,b,a;

			rgba() throw();
			rgba( const T R, const T G, const T B) throw();
			rgba( const T R, const T G, const T B, const T A) throw();
			rgba( const rgba &) throw();
			rgba( const rgba &, const T A ) throw();

			static rgba ramp(T v,const T vmin=0,const T vmax=1) throw();

		};

		struct rgba32
		{
			uint8_t r,g,b,a;
			template <typename T>
			static rgba32 ramp( const rgba32 colors[256], T v, T vmin=0, T vmax=1) throw();
		};
		
		
		
		#if defined(__DMC__)
		template class rgba<double>;
		template class rgba<float>;
		#endif

	}
}

YOCTO_SUPPORT_C_STYLE_OPS(color::rgba<float>)
YOCTO_SUPPORT_C_STYLE_OPS(color::rgba<double>)

#endif
