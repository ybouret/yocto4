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
			typedef workspace<T,array3D,U,region3D>  wksp_type;
			typedef typename wksp_type::param_coord  param_coord;
			typedef typename wksp_type::param_vertex param_vertex;
			typedef typename wksp_type::axis_type    axis_type;
			
			explicit wksp3D(param_coord  lo, 
							param_coord  hi, 
							param_coord  ghosts_lo, 
							param_coord  ghosts_up,
							param_vertex vmin,
							param_vertex vmax,
							size_t       a,
							size_t       b,
							const char  *names_list[] 
							) :
			wksp_type(lo,hi,ghosts_lo,ghosts_up,vmin,vmax,a,b,names_list)
			{
			}
			
			virtual ~wksp3D() throw()
			{
			}
			
			inline const axis_type & X() const throw() { return this->axis(0); }
			inline const axis_type & Y() const throw() { return this->axis(1); }
			inline const axis_type & Z() const throw() { return this->axis(2); }

		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(wksp3D);
		};
		
	}
	
}


#endif
