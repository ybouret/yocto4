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

			const axis_type &X, &Y, &Z;
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
			wksp_type(lo,hi,ghosts_lo,ghosts_up,vmin,vmax,a,b,names_list),
				X( this->axis(0) ),
				Y( this->axis(1) ),
				Z( this->axis(2) )
			{
			}

			virtual ~wksp3D() throw()
			{
			}

			virtual void load_offsets( offsets_list &offsets, const layout3D &sub )
			{
				assert( this->outline.has(sub.lower) );
				assert( this->outline.has(sub.upper) );
				size_t        zoff  = sub.lower.z - this->outline.lower.z;
				const  size_t dz    = this->outline.width.x * this->outline.width.y;
				const size_t  yoff0 = sub.lower.y - this->outline.lower.y;
				const size_t dy     = this->outline.width.x;
				const size_t xoff0  = sub.lower.x - this->outline.lower.x;
				for( size_t k = sub.width.z; k>0; --k, zoff += dz )
				{
					size_t yoff = zoff + yoff0;
					for( size_t j = sub.width.y; j>0; --j, yoff += dy )
					{
						size_t xoff = yoff + xoff0;
						for( size_t i = sub.width.x; i>0; --i, ++xoff )
						{
							assert(xoff<this->outline.items);
							(void) offsets.insert(xoff);
						}
					}
				}
			}

		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(wksp3D);
		};

	}

}


#endif
