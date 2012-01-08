#ifndef YOCTO_CLIFF_CONTOUR_INCLUDED
#define YOCTO_CLIFF_CONTOUR_INCLUDED 1

#include "yocto/cliff/array2d.hpp"

namespace yocto
{
	
	namespace cliff
	{
		template <typename T>
		struct contour
		{
			typedef typename vertex2D<T>::type vertex_t;
			typedef functor<void,TL3(const vertex_t &, const vertex_t &,T)> callback;
			static void compute(const array2D<T> &d,
								const array1D<T> &x, 
								const array1D<T> &y,
								const layout2D   &sub,
								const array<T>   &z,
								callback          proc);
		};
		
	}
	
}

#endif
