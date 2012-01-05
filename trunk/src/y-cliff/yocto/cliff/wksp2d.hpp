#ifndef YOCTO_CLIFF_WKSP2D_INCLUDED
#define YOCTO_CLIFF_WKSP2D_INCLUDED 1

#include "yocto/cliff/workspace.hpp"
#include "yocto/cliff/array2d.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		
		
		template <typename T, typename U>
		class wksp2D : public workspace<T,array2D,U,region2D>
		{
		public:
			typedef workspace<T,array2D,U,region2D>  wksp_type;
			typedef typename wksp_type::param_coord  param_coord;
			typedef typename wksp_type::param_vertex param_vertex;
			typedef typename wksp_type::axis_type    axis_type;
			
			explicit wksp2D(param_coord  lo, 
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
			
			virtual ~wksp2D() throw()
			{
			}
			
			inline const axis_type & X() const throw() { return this->axis(0); }
			inline const axis_type & Y() const throw() { return this->axis(1); }
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(wksp2D);
		};
		
	}
	
}


#endif
