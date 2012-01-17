#include "yocto/cliff/in1d.hpp"
#include "yocto/cliff/in2d.hpp"
#include "yocto/cliff/in3d.hpp"

namespace yocto
{

	namespace cliff
	{
		
		template <>
		void layout<coord1D>:: load_offsets( offsets_list &offsets, const layout<coord1D> &sub, const layout<coord1D> &outline ) 
		{
			assert( outline.has( sub.lower ) );
			assert( outline.has( sub.upper ) );
			assert( sub.lower >= outline.lower );
			size_t xoff = sub.lower - outline.lower;
			for( size_t i=sub.width; i>0; --i ) (void)offsets.insert( xoff++ );
		}
		
		template <>
		void layout<coord2D>:: load_offsets( offsets_list &offsets, const layout<coord2D> &sub, const layout<coord2D> &outline ) 
		{
			assert( outline.has( sub.lower ) );
			assert( outline.has( sub.upper ) );
			const unit_t xoff0 = sub.lower.x - outline.lower.x;
			size_t       yoff  = sub.lower.y - outline.lower.y;
			const size_t dy    = outline.width.x;
			for( size_t j =  sub.width.y; j>0; --j, yoff += dy )
			{
				size_t ans = yoff + xoff0;
				for( size_t i = sub.width.x; i>0;--i, ++ans )
				{
					assert( ans < outline.items );
					(void) offsets.insert( ans );
				}
			}
		}
		
		
		template <>
		void layout<coord3D>:: load_offsets( offsets_list &offsets, const layout<coord3D> &sub, const layout<coord3D> &outline ) 
		{
			assert( outline.has( sub.lower ) );
			assert( outline.has( sub.upper ) );
			size_t        zoff  = sub.lower.z - outline.lower.z;
			const  size_t dz    = outline.width.x * outline.width.y;
			const size_t  yoff0 = sub.lower.y - outline.lower.y;
			const size_t dy     = outline.width.x;
			const size_t xoff0  = sub.lower.x - outline.lower.x;
			for( size_t k = sub.width.z; k>0; --k, zoff += dz )
			{
				size_t yoff = zoff + yoff0;
				for( size_t j = sub.width.y; j>0; --j, yoff += dy )
				{
					size_t xoff = yoff + xoff0;
					for( size_t i = sub.width.x; i>0; --i, ++xoff )
					{
						assert(xoff<outline.items);
						(void) offsets.insert(xoff);
					}
				}
			}
			
		}
		
		
	}
	
}