#include "yocto/fragment/layout.hpp"
#include "yocto/code/utils.hpp"


namespace yocto {
	
	namespace fragment
	{
		
		layout:: layout( size_t user_block_size, size_t user_round_log2 ) throw() :
		round_log2( clamp<size_t>( round_log2_min, user_round_log2, round_log2_max ) ),
		round_byte( 1 << round_log2 ),
		round_mask( round_byte - 1  ),
		round_not2( ~round_mask     ),
		block_size( (max_of<size_t>(user_block_size,1)+round_mask) & round_not2 )
		{
			
		}
		
		layout:: ~layout() throw()
		{
			
		}
		
		size_t layout:: round( size_t n ) const throw()
		{
			return ( n + round_mask ) & round_not2;
		}
		
		layout:: layout( const layout &f ) throw() :
		round_log2( f.round_log2 ),
		round_byte( f.round_byte ),
		round_mask( f.round_mask ),
		round_not2( f.round_not2 ),
		block_size( f.block_size )
		{
		}
		
		void layout:: process(  void *huge_data, size_t data_size, callback proc, void *args) const
		{
			assert( proc!=NULL);
			if( data_size > 0 )
			{
				uint8_t *frag_data = (uint8_t *)huge_data;
				/* find dataWrap = 2^(log2frag) * block_size */
				size_t data_wrap =  block_size; /* (2^0) * block_size */
				size_t log2_frag = 0;
				size_t frag_size = block_size;
				while( data_wrap < data_size ) {
					data_wrap <<= 1;
					++log2_frag;
				}
				
				/* initialize fragment size */
				while( data_size >  block_size ) {
					
					/* stabilize by recomputing */
					if( data_wrap > data_size )
					{
						frag_size = data_size >> log2_frag;
						frag_size = ( frag_size + round_mask ) & round_not2;
						--log2_frag;
						data_wrap >>= 1;
					}
					proc( frag_data, frag_size, args);
					data_size -= frag_size;
					frag_data += frag_size;
				}
				
				if( data_size )
				{
					proc( frag_data, data_size, args); //!< last block
				}
			}
		}
		
		
	}
	
}
