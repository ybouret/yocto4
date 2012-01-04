#include "yocto/cliff/array3d.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/memory/global.hpp"

namespace yocto
{
	namespace cliff
	{
		using math::z_type;
		
		template <>
		array3D<z_type>:: array3D( const layout3D &L ) :
		linear_type(L),
		slice_layout( coord2D(lower.x,lower.y), coord2D(upper.x,upper.y) ),
		slices( width.z ),
		slice( memory::kind<memory::global>::acquire_as<slice_type>(slices) - lower.z )
		{
			unit_t z=lower.z;
			try
			{
				while( z <= upper.z )
				{
					new( &slice[z] ) slice_type( slice_layout );
					++z;
				}
			}
			catch(...)
			{
				while( z > lower.z )
				{
					--z;
					slice[z].~slice_type();
				}
				slice += lower.z;
				memory::kind<memory::global>::release_as<slice_type>(slice,slices); 
				throw;
			}
			
		}
		
		
		template <>
		array3D<z_type>:: ~array3D( )  throw()
		{
			for( unit_t z = upper.z; z >= lower.z; --z )
			{
				slice[z].~slice_type();
			}
			slice += lower.z;
			memory::kind<memory::global>::release_as<slice_type>(slice,slices); 
		}
		
		
		template <>
		void array3D<z_type>:: link( z_type *addr ) throw()
		{
			assert( addr != NULL );
			entry = addr;
			for( unit_t z = lower.z; z <= upper.z; ++z, addr += slice_layout.items )
			{
				slice[z].link( addr );
			}
		}

		template <>
		array3D<z_type>::slice_type & array3D<z_type>:: operator[]( unit_t z ) throw()
		{
			assert( z >= lower.z );
			assert( z <= upper.z );
			return slice[z];
		}
		
		template <>
		const array3D<z_type>::slice_type & array3D<z_type>:: operator[]( unit_t z ) const throw()
		{
			assert( z >= lower.z );
			assert( z <= upper.z );
			return slice[z];
		}
		
	}

}

