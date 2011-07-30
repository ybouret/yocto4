#include "yocto/math/ztype.hpp"
#include "yocto/spade/array3d.hpp"

#include "yocto/code/round.hpp"

namespace yocto
{
	using namespace math;
	
	namespace spade
	{
		
		template <>
		array3D<z_type>:: ~array3D() throw()
		{}
		
		template <>
		array3D<z_type>:: array3D( const layout3D &L ) throw() :
		linear_type( L ),
		slice_( NULL ),
		slice_layout( layout2D( coord2D( lower.x, lower.y), coord2D( upper.x, upper.y) ) ),
		slice_bytes(  slice_layout.items * sizeof(z_type) )
		{
		}
		
		template <>
		array3D<z_type>::slice_type &  array3D<z_type>:: operator[]( unit_t z ) throw()
		{
			assert( slice_ != NULL );
			assert( entry  != NULL );
			assert( z >= lower.z );
			assert( z <= upper.z );
			
			return slice_[ z ];
		}
		
		template <>
		const array3D<z_type>::slice_type &  array3D<z_type>:: operator[]( unit_t z ) const throw()
		{
			assert( slice_ != NULL );
			assert( entry  != NULL );
			assert( z >= lower.z );
			assert( z <= upper.z );
			
			return slice_[ z ];
		}
		
		template<>
		array3D<z_type>::type &    array3D<z_type>::   operator[]( const coord3D & p ) throw()
		{
			assert( this->has( p ) );
			assert( slice_ != NULL );
			assert( entry  != NULL );
			return slice_[ p.z ][ p.y ][ p.x ];
		}
		
		template<>
		array3D<z_type>::const_type &    array3D<z_type>::   operator[]( const coord3D & p ) const throw()
		{
			assert( this->has( p ) );
			assert( slice_ != NULL );
			assert( entry  != NULL );
			return slice_[ p.z ][ p.y ][ p.x ];
		}
		
		template <>
		void array3D<z_type>:: request( size_t lengths[] ) const throw()
		{
			assert( lengths != NULL );
			
			//-- memory for slices
			const size_t memory_for_slices = width.z * sizeof(slice_type );
			lengths[0] = YOCTO_MEMALIGN( memory_for_slices );
			
			//-- memory for rows
			const size_t num_rows        = width.z * width.y;
			const size_t memory_for_rows = num_rows * sizeof( row_type );
			lengths[1] = YOCTO_MEMALIGN( memory_for_rows );
			
			//-- memory for linear space
			lengths[2] = bytes;
			
			
		}
		
		template <>
		void array3D<z_type>:: link_to( void * buffers[] ) throw()
		{
			assert( NULL == entry  );
			assert( NULL == slice_ );
			assert( buffers );
			assert( buffers[0] );
			assert( buffers[1] );
			assert( buffers[2] );
			
			entry  = (z_type *)     buffers[2];
			slice_ = (slice_type *) buffers[0]; 
			slice_ -= lower.z;
			
			const size_t rows_per_slice   = width.y; 
			const size_t shift_rows       = rows_per_slice * sizeof(row_type);
			void *       slice_buffers[2] = { buffers[1], entry };
			
			for( unit_t z = lower.z; z <= upper.z; ++z )
			{
				slice_type *s = new ( &slice_[z] ) slice_type( slice_layout );
				s->link_to( slice_buffers );
				
				update( slice_buffers[0], shift_rows  );
				update( slice_buffers[1], slice_bytes );
			}
			
		}
		
		template <>
		void array3D<z_type> :: copy_into( array3D &sub ) const throw()
		{
			assert( this->has( sub.lower ) );
			assert( this->has( sub.upper ) );
			
			coord3D p    = sub.lower;
			while(  p.z <= sub.upper.z )
			{
				
				++p.z;
			}
			
		}
		
		template <>
		void array3D<z_type> :: fill_with( const array3D &sub ) throw()
		{
			assert( this->has( sub.lower ) );
			assert( this->has( sub.upper ) );
			
			coord3D p    = sub.lower;
			while(  p.z <= sub.upper.z )
			{
				
				++p.z;
			}
			
		}
		

		
	}
}
