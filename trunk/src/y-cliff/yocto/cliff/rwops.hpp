#ifndef YOCTO_CLIFF_RWOPS_INCLUDED
#define YOCTO_CLIFF_RWOPS_INCLUDED 1

#include "yocto/cliff/wksp2d.hpp"
#include "yocto/cliff/wksp3d.hpp"

namespace yocto
{
	namespace cliff
	{
		
		template <typename T>
		struct rwops
		{
			typedef wksp3D<T,T>                wksp3_type;
			typedef typename vertex3D<T>::type vertex3D_t;
			typedef triangle3D<T>              triangle3D_t;
			typedef typename vertex2D<T>::type vertex2D_t;
			
			static void save_vtk(const string        &filename,
								 const string        &title,
								 const wksp3_type    &wksp,
								 const array<size_t> &cid,
								 const layout3D      &sub);
			
			typedef wksp2D<T,T>                wksp2_type;
			static void save_vtk(const string        &filename,
								 const string        &title,
								 const wksp2_type    &wksp,
								 const array<size_t> &cid,
								 const layout2D      &sub);
			
			static void save_vtk( const string &filename, const string &title, const array<triangle3D_t> &triangles);
			
			
		};
	}
}


#endif
