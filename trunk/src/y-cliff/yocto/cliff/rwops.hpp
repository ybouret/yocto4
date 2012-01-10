#ifndef YOCTO_CLIFF_RWOPS_INCLUDED
#define YOCTO_CLIFF_RWOPS_INCLUDED 1


#include "yocto/cliff/array3d.hpp"

namespace yocto
{
	namespace cliff
	{
		
		template <typename T>
		struct rwops
		{
			typedef typename vertex3D<T>::type vertex3D_t;
			
			static void save_vtk(const string     &filename,
								 const string     &name,
								 const array3D<T> &field,
								 const layout3D   &sub,
								 const vertex3D_t &origin,
								 const vertex3D_t &spacing );
			
			
		};
	}
}


#endif
