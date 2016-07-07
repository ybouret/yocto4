#include "yocto/pack/bwt.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/unroll.hpp"
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
            assert(!(NULL==output&&size>0));
            assert(!(NULL==input&&size>0));
            
			const uint8_t    *buf_in  = (const uint8_t *)input;
			uint8_t          *buf_out = (uint8_t       *)output;
			

            if(size>0)
			{
#define YBWT_INIT_INDEX(I) indices[I] = I
                YOCTO_LOOP_FUNC_(size,YBWT_INIT_INDEX,0);
                lw_array<size_t>  arr( indices, size );
                rotlexdat         cmp = { buf_in, size };
                quicksort( arr, cmp );
			}
            
            size_t       pidx = 0;
            const size_t shft = size-1;
            size_t       ii   = 0;
            
			for(; ii < size; ++ii )
			{
				const size_t idx = indices[ii];
                buf_out[ii] = buf_in[ (idx+shft) % size];
				if( 0 == idx )
                {
					pidx=ii;
                    break;
                }
			}
            
            for(;ii<size;++ii)
            {
                buf_out[ii] = buf_in[ (indices[ii]+shft) % size];
            }
            
            return pidx;
		}
        
        
		void   bwt:: decode( void *output, const void *input, const size_t size, size_t *indices, const size_t primary_index) throw()
		{
            assert(!(NULL==output&&size>0));
            assert(!(NULL==input&&size>0));
            
			size_t         buckets[256];
			const uint8_t *buf_in  = (const uint8_t *)input;
			uint8_t       *buf_out = (uint8_t *)output;
            memset(buckets,0,sizeof(buckets));
            
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
			for( size_t i=size;i>0;--i)
            {
				const uint8_t bj = buf_in[j];
				*(--c) = bj;
				j = buckets[bj] + indices[j];
			}
            
            
		}
        
        bwt::common16:: common16() :
        numindx(65536),
        indices( memory::kind<memory::global>::acquire_as<size_t>(numindx) )
        {
        }
        
        bwt:: common16:: ~common16() throw()
        {
            memory::kind<memory::global>::release_as<size_t>(indices,numindx);
        }
        
        uint16_t bwt::common16:: encode( void *output, const void *input, const size_t size) throw()
        {
            assert(!(NULL==output&&size>0));
            assert(!(NULL==input&&size>0));
            assert(size<=65536);
            return uint16_t( bwt::encode(output, input, size, indices) );
        }

        
        void bwt::common16:: decode( void *output, const void *input, const size_t size, const uint16_t primary_index) throw()
        {
            assert(!(NULL==output&&size>0));
            assert(!(NULL==input&&size>0));
            assert(size<=65536);
            
            bwt::decode(output, input, size, indices, primary_index);
        }

        
    }
}

