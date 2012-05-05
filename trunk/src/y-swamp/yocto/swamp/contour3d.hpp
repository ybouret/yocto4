#ifndef YOCTO_CONTOUR3D_INCLUDED
#define YOCTO_CONTOUR3D_INCLUDED 1

#include "yocto/swamp/array3d.hpp"
#include "yocto/swamp/triangle.hpp"
#include "yocto/swamp/level-set.hpp"

namespace yocto
{
	
	namespace swamp
	{
		
		//! marching cubes algortihm
		template <typename T>
		struct contour3D
		{
			typedef functor<void,TL2(const triangle3D<T> &, const level<T> &)> callback;
			
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
