#include "yocto/cliff/array2d.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/memory/global.hpp"

namespace yocto
{
	namespace cliff
	{
		using math::z_type;
		
		template <>
		array2D<z_type>:: array2D( const layout2D &L) :
		linear_type(L),
		row_layout( lower.x, upper.x ),
		rows( row_layout.width ),
		row( memory::kind<memory::global>::acquire_as<row_type>(rows) - lower.y )
		{
			for( unit_t y = lower.y; y <= upper.y; ++y )
				new ( &row[y] ) row_type( row_layout );
			
		}
		
		
		template <>
		array2D<z_type>:: ~array2D() throw()
		{
			for( unit_t y = upper.y; y >= lower.y; --y)
			{
				row[y].~row_type();
			}
			row += lower.y;
			memory::kind<memory::global>::release_as<row_type>(row,rows) ;
		}
		
		
		template <>
		void array2D<z_type>:: link( z_type *addr ) throw()
		{
			entry = addr;
			for( unit_t y = lower.y; y <= upper.y; ++y, addr += width.x )
				row[y].link(addr);
		}
		
	}
	
}


