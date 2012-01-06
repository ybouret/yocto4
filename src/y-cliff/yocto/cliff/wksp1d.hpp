#ifndef YOCTO_CLIFF_WKSP1D_INCLUDED
#define YOCTO_CLIFF_WKSP1D_INCLUDED 1

#include "yocto/cliff/workspace.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		
		
		template <typename T, typename U>
		class wksp1D : public workspace<T,array1D,U,region1D>
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef workspace<T,array1D,U,region1D>  wksp_type;
			typedef typename wksp_type::param_coord  param_coord;
			typedef typename wksp_type::param_vertex param_vertex;
			typedef typename wksp_type::axis_type    axis_type;
			const axis_type &X;
			explicit wksp1D(param_coord  lo, 
							param_coord  hi, 
							param_coord  ghosts_lo, 
							param_coord  ghosts_up,
							param_vertex vmin,
							param_vertex vmax,
							size_t       a,
							size_t       b,
							const char  *names_list[] 
							) :
			wksp_type(lo,hi,ghosts_lo,ghosts_up,vmin,vmax,a,b,names_list),
			X( this->axis(0) )
			{
			}
			
			virtual ~wksp1D() throw()
			{
			}
			
						
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(wksp1D);
		};
		
	}
	
}


#endif
