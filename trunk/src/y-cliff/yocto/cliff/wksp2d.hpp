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
			
			void scan( const layout2D &sub, const array<size_t> &comp )
			{
				assert( comp.size() > 0 );
				assert( this->has(sub.lower) );
				assert( this->has(sub.upper) );
				const size_t n = comp.size();
				vector<T>    a(n,T(0));
				for( size_t y=sub.lower.y; y <= sub.upper.y; ++y )
				{
					for( unit_t x=sub.lower.x; x <= sub.upper.x; ++x )
					{
						for( size_t j=1; j <= n; ++j )
						{
							a[j] = (*this)[ comp[j] ][y][x];
						}
					}
				}
				
			}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(wksp2D);
		};
		
	}
	
}


#endif
