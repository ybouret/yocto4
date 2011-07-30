#include "yocto/math/ztype.hpp"
#include "yocto/spade/link1d.hpp"

namespace yocto
{
	using namespace math;
	
	namespace spade
	{
		
		
		template <>
		void link1d<z_type>( vector1d_type &x_data, array1D<z_type> &X )
		{
			size_t length = 0;
			X.request( &length );
			x_data.make( length, 0 );
			void * buffers[] = { x_data(0) };
			X.link_to( buffers );
		}
		
		template <>
		void link1d<z_type>( vector1d_type &xy_data, array1D<z_type> &X, array1D<z_type> &Y )
		{
			size_t x_length = 0;
			X.request( &x_length );
			
			size_t y_length = 0;
			Y.request( &y_length );
			const size_t  xy_len = x_length + y_length; 
			xy_data.make( xy_len, 0 );
			void * buffers[] = { xy_data(0), xy_data(x_length) };
			X.link_to( buffers   );
			Y.link_to( buffers+1 );
		}
		
		
		
	}
	
}
