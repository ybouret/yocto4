#include "yocto/mpk/natural.hpp"
#include "yocto/ios/bitio.hpp"

#include "yocto/code/round.hpp"

namespace yocto
{
	namespace mpk
	{
		bool natural::bit( const size_t index ) const throw()
		{
			const size_t n = index >> 3;
			if( n >= size_ )
				return false;
			else
			{
				return ( byte_[n] & _bit[ index & 7 ] ) != 0;
			}
		}
		
		void natural:: store( ios::bitio &out, size_t nbits ) const
		{
			size_t count = 0;
			try
			{
				for( ; count < nbits; ++count )
				{
					out.push_back( bit(count) );
				}
			}
			catch(...)
			{
				while( count-- > 0 )
					out.pop_back();
				throw;
			}
		}
		
		natural natural:: query( ios::bitio &in, size_t nbits ) 
		{
			assert(nbits<=in.size());
			natural ans( YOCTO_ROUND8(nbits) >> 3, as_capacity );
			
			uint8_t *b = ans.byte_;
			for( size_t i = 0; i < nbits; ++i )
			{
				if(  in.front() )
				{
					assert( (i>>3) < ans.maxi_ );
					b[i>>3] |= (1 << ( i & 7 ));
					
				}
				in.pop_front();
			}
			
			ans.update();
			return ans;
		}
		
		
	}
}

