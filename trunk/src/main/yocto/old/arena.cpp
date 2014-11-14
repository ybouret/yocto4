#include "yocto/memory/arena.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/memory/chunk.hpp"

#if !defined(NDEBUG)
#include <iostream>
#endif

namespace yocto
{
	namespace memory
	{
		
		arena:: ~arena() throw()
		{
			while( size_ > 0 )
			{
				assert( chunk_ );
				chunk *ch = &chunk_[ --size_ ];
#if !defined(NDEBUG)
				if( ch->stillAvailable < ch->providedNumber )
				{
					std::cerr << "** [memory::arena]  block_size " << block_size << ": missing #" << int(ch->providedNumber-ch->stillAvailable) << std::endl;
				}
#endif
				size_t csz = chunk_size;
				kind<global>::release_as<uint8_t>( ch->data, csz );
			}
			kind<global>::release_as<chunk>( chunk_, maxi_ );
		}
		
		arena:: arena( const size_t a_block_size, const size_t a_num_blocks ) throw() :
		chunk_( NULL ),
		size_(0),
		maxi_(0),
		accessible_( 0 ),
		pAcquiring_( NULL ),
		pReleasing_( NULL ),
		chunk_last_( NULL ),
		pAvailable_( NULL ),
		block_size( a_block_size ),
		num_blocks( a_num_blocks ),
		chunk_size( block_size * num_blocks ),
		new_blocks_( num_blocks - 1)
		{
			assert( block_size > 0 );
			assert( num_blocks >= chunk::num_blocks_min);
			assert( num_blocks <= chunk::num_blocks_max);
		}
		
	}
	
	
}
