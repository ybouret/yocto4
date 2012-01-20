#ifndef YOCTO_CLIFF_WKSP3D_INCLUDED
#define YOCTO_CLIFF_WKSP3D_INCLUDED 1

#include "yocto/cliff/workspace.hpp"
#include "yocto/cliff/array3d.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		
		
		template <typename T, typename U>
		class wksp3D : public workspace<T,array3D,U,region3D>
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef workspace<T,array3D,U,region3D>  wksp_type;
			typedef typename wksp_type::param_coord  param_coord;
			typedef typename wksp_type::param_vertex param_vertex;
			typedef typename wksp_type::axis_type    axis_type;
			typedef typename wksp_type::layout_type  layout_type;
			typedef typename wksp_type::region_type  region_type;
			typedef typename wksp_type::ghosts_type  ghosts_type;
			
			const axis_type &X, &Y, &Z;
			explicit wksp3D(const layout_type &L,
							const ghosts_type &G,
							const region_type &R,
							size_t             a,
							size_t             b,
							const char        *names_list[] 
							) :
			wksp_type(L,G,R,a,b,names_list),
			X( this->axis(0) ),
			Y( this->axis(1) ),
			Z( this->axis(2) )
			{
			}
			
			virtual ~wksp3D() throw()
			{
			}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(wksp3D);
		};
		
	}
	
}


#endif
