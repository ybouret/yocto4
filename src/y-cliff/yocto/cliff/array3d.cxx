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
		
		template <>
		void array3D<z_type>:: save( array3D<z_type> &target, const layout3D &sub) const throw()
		{
			assert(  this->has( sub.lower ) );
			assert(  this->has( sub.upper ) );
			assert( target.has( sub.lower ) );
			assert( target.has( sub.upper ) );
			
			const array3D<z_type> &self = *this;
			for( unit_t z=sub.upper.z; z >= sub.lower.z; --z )
			{
				const slice_type &s_src = self[z];
				slice_type       &s_tgt = target[z];
				for( unit_t y=sub.upper.y; y >= sub.lower.y; --y )
				{
					const array1D<z_type> &r_src = s_src[y];
					array1D<z_type>       &r_tgt = s_tgt[y];
					for( unit_t x=sub.upper.x; x >= sub.lower.x; --x )
					{
						r_tgt[x] = r_src[x];
					}
				}
			}
			
		}
		
		template <>
		void array3D<z_type>:: load( const array3D<z_type> &source, const layout3D &sub) throw()
		{
			
			assert(  this->has( sub.lower ) );
			assert(  this->has( sub.upper ) );
			assert( source.has( sub.lower ) );
			assert( source.has( sub.upper ) );
			
			array3D<z_type> &self = *this;
			for( unit_t z=sub.upper.z; z >= sub.lower.z; --z )
			{
				const slice_type &s_src = source[z];
				slice_type       &s_tgt = self[z];
				for( unit_t y=sub.upper.y; y >= sub.lower.y; --y )
				{
					const array1D<z_type> &r_src = s_src[y];
					array1D<z_type>       &r_tgt = s_tgt[y];
					for( unit_t x=sub.upper.x; x >= sub.lower.x; --x )
					{
						r_tgt[x] = r_src[x];
					}
				}
			}
			
		}
		
		
	}
	
}

