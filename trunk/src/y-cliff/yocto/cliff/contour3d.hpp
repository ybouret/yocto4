#ifndef YOCTO_CONTOUR3D_INCLUDED
#define YOCTO_CONTOUR3D_INCLUDED 1

#include "yocto/cliff/array3d.hpp"
#include "yocto/cliff/level-set.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		template <typename T>
		struct contour3D
		{
			typedef typename level_set<T>::level level_type;
			typedef functor<void,TL2(const triangle3D<T> &, const level_type &)> callback;
			
			static void compute(const array3D<T>   &d, 
								const array1D<T>   &x, 
								const array1D<T>   &y,
								const array1D<T>   &z,
								const layout3D     &sub,
								const level_set<T> &levels,
								callback           &proc);
		};
	}
	
	
}

#endif
