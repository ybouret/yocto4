//! \file

#ifndef YOCTO_CLIFF_CONTOUR2D_INCLUDED
#define YOCTO_CLIFF_CONTOUR2D_INCLUDED 1

#include "yocto/cliff/array2d.hpp"
#include "yocto/cliff/level-set.hpp"

namespace yocto
{
	
	namespace cliff
	{
		//! contouring algorithm
		template <typename T>
		struct contour2D
		{
			typedef typename level_set<T>::level level_type;
			typedef typename vertex2D<T>::type   vertex_t;
			
			typedef functor<void,TL3(const vertex_t &, const vertex_t &,const level_type &)> callback;
			
			static void compute(const array2D<T>   &d,
								const array1D<T>   &x, 
								const array1D<T>   &y,
								const layout2D     &sub,
								const level_set<T> &levels,
								callback            proc);
			
			
			
		};
		
	}
	
}

#endif
