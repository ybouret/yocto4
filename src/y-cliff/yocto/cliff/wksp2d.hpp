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
			
			typedef functor<type,TL2(U,U)> function;
			
			inline void fill( size_t var, const layout2D &L, function &f )
			{
				assert( this->outline.has(L.lower) );
				assert( this->outline.has(L.upper) );
				assert( var >= this->cmin );
				assert( var <= this->cmax );
				array2D<T>      & F = (*this)[ var ];
				for( unit_t y=L.lower.y; y <= L.upper.y; ++y)
				{
					array1D<T> &F_y = F[y];
					const U      _y = Y[y];
					for( unit_t x=L.lower.x; x <= L.upper.x; ++x )
					{
						F_y[ x ] = f( X[x], _y );
					}
				}
			}
			
			inline void fill( const string &id, const layout2D &L, function &f )
			{
				const components &comp = *this;
				fill( comp(id), L, f );
			}
			
			inline void fill( const char *id, const layout2D &L, function &f )
			{
				const components &comp = *this;
				fill( comp(id), L, f );
			}
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(wksp2D);
		};
		
	}
	
}


#endif
