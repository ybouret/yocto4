
#include "yocto/math/ztype.hpp"
#include "yocto/spade/array1d.hpp"
#include "yocto/code/round.hpp"

#include <cstring>
#include <iostream>

namespace yocto
{
	using math::z_type;
	
	namespace spade
	{
		
		template <>
		array1D<z_type>:: array1D( const layout1D &L ) throw() :
		linear_type( L ),
		item_( NULL )
		{
		}
		
		
		template <>
		array1D<z_type>:: ~array1D() throw()
		{
			
		}
		
		template <>
		array1D<z_type>::type & array1D<z_type>:: operator[]( unit_t x ) throw()
		{
			assert( x >= lower );
			assert( x <= upper );
			assert( has(x) );
			
			assert( item_  != NULL );
			assert( entry  != NULL );
			assert( item_ + lower == entry );
			
			return item_[ x ];
		}
		
		template <>
		array1D<z_type>::const_type & array1D<z_type>:: operator[]( unit_t x ) const throw()
		{
			assert( x >= lower );
			assert( x <= upper );
			assert( has(x)     );
			assert( item_ != NULL );
			assert( entry != NULL );
			assert( item_ + lower == entry );
			
			return item_[ x ];
		}
		
		template <>
		void array1D<z_type>:: request( size_t lengths[] ) const throw()
		{
			assert( lengths );
			lengths[0] = bytes;
		}
	
		template <>
		void array1D<z_type>:: link_to( void * buffers[]  ) throw()
		{
			assert( buffers    != NULL );
			assert( buffers[0] != NULL );
			assert( NULL == entry );
			
			entry = static_cast<z_type *>( buffers[0] );
			item_ = entry - lower;
			
		}
		
		template <>
		void array1D<z_type>:: copy_into( array1D &sub ) const throw()
		{
			assert( this->has( sub.lower ) );
			assert( this->has( sub.upper ) );
			memcpy( sub.entry,  & item_[ sub.lower ], sub.bytes );
		}
		
		template <>
		void array1D<z_type>:: fill_with( const array1D &sub )  throw()
		{
			assert( this->has( sub.lower ) );
			assert( this->has( sub.upper ) );
			memcpy( & item_[ sub.lower ], sub.entry, sub.bytes );
		}
		
	}
	
}
