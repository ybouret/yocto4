#ifndef YOCTO_CLIFF_WKSP2D_INCLUDED
#define YOCTO_CLIFF_WKSP2D_INCLUDED 1

#include "yocto/cliff/workspace.hpp"
#include "yocto/cliff/array2d.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		
		
		template <typename T, typename U>
		class wksp2D : public workspace<T,array2D,U,region2D>
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef workspace<T,array2D,U,region2D>  wksp_type;
			typedef typename wksp_type::param_coord  param_coord;
			typedef typename wksp_type::param_vertex param_vertex;
			typedef typename wksp_type::axis_type    axis_type;
			
			const axis_type &X, &Y;
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
			wksp_type(lo,hi,ghosts_lo,ghosts_up,vmin,vmax,a,b,names_list),
			X( this->axis(0) ),
			Y( this->axis(1) )
			{
			}
			
			virtual ~wksp2D() throw()
			{
			}
			
			virtual void load_offsets( offsets_type offsets, const layout2D &sub )
			{
				assert( this->outline.has(sub.lower) );
				assert( this->outline.has(sub.upper) );
				const unit_t xoff0 = sub.lower.x - this->outline.lower.x;
				size_t       yoff  = sub.lower.y - this->outline.lower.y;
				const size_t dy    = this->outline.width.x;
				for( size_t j =  sub.width.y; j>0; --j, yoff += dy )
				{
					size_t ans = yoff + xoff0;
					for( size_t i = sub.width.x; i>0;--i, ++ans )
					{
						assert( ans < this->outline.items );
						(void) offsets.insert( ans );
					}
				}
			}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(wksp2D);
		};
		
	}
	
}


#endif
