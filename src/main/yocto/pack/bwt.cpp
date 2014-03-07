#include "yocto/pack/bwt.hpp"
#include "yocto/sequence/c-array.hpp"
#include "yocto/sort/quick.hpp"

#include <cstring>

namespace yocto
{
	
	namespace pack
	{
		namespace
		{
			struct rotlexdat
			{
				const uint8_t *buf;
				size_t         len;
				
				
				inline int operator()( size_t li, size_t ri ) const throw()
				{
					
					if( li == ri )
						return 0;
					
					size_t ac = len;
					while( buf[li] == buf[ri] )
					{
						li = (li+1) % len;
						ri = (ri+1) % len;
						if( --ac == 0 )
							return 0;
					}
					return (buf[li] > buf[ri]) ? 1 : -1;
				}
			};
		}
		
		size_t bwt::encode( void *output, const void *input, const size_t size, size_t *indices) throw()
		{
			const uint8_t    *buf_in  = (const uint8_t *)input;
			uint8_t          *buf_out = (uint8_t *)output;
			
			for( size_t i=0; i < size; ++i )
				indices[i] = i;
			{
                c_array<size_t>  arr( indices, size );
                rotlexdat        cmp = { buf_in, size };
                quicksort( arr, cmp );
			}
            
			size_t pidx=0;
			for( size_t i=0; i < size; ++i )
			{
				const size_t idx = indices[i];
				buf_out[i] = buf_in[ ((idx+size)  -1) % size];
				if( 0 == idx )
					pidx=i;
			}
            return pidx;
		}
		
		void   bwt:: decode( void *output, const void *input, const size_t size, size_t *indices, size_t primary_index) throw()
		{
			size_t buckets[256];
			const uint8_t *buf_in  = (const uint8_t *)input;
			uint8_t       *buf_out = (uint8_t *)output;
			memset( buckets, 0, sizeof(buckets) );
			
            
			for( size_t i=0; i < size; ++i )
			{
				const size_t bi = buf_in[i];
				indices[i] = buckets[bi];
				buckets[bi]++;
			}
			
			size_t sum = 0;
			for( size_t i=0; i < 256; ++i )
			{
				size_t      &__b = buckets[i];
				const size_t __t = __b;
				__b = sum;
				sum += __t;
			}
			
			size_t      j = primary_index;
			uint8_t    *c = buf_out + size;
			for( size_t i=size;i>0;--i) {
				const uint8_t bj = buf_in[j];
				*(--c) = bj;
				j = buckets[bj] + indices[j];
			}
            
            
		}
		
	}
}

