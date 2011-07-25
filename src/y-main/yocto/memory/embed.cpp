#include "yocto/memory/embed.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/unroll.hpp"


namespace yocto
{
	
	namespace memory
	{
		static inline size_t embed_round( size_t pos ) throw()
		{
			return YOCTO_MEMALIGN( pos );
		}
		
		size_t embed:: prepare( embed reg[], size_t num ) throw()
		{
			assert( !(num>0 && NULL == reg) );
			if( num > 0 )
			{
				assert(NULL != reg[0].hook);
				assert(NULL == * (reg[0].hook) );
				reg[0].offset = 0;
				const embed *prev = &reg[0];
				embed       *curr = &reg[1];
				
				YOCTO_LOOP(num-1,
						   assert(NULL !=   curr->hook );
						   assert(NULL == *(curr->hook));
						   curr->offset = embed_round(prev->offset+prev->length);
						   ++curr;
						   ++prev;
						   );
				assert( prev == &reg[num-1] );
				return embed_round(prev->offset+prev->length);
			}
			else
				return 0;
		}
		
		void embed:: dispatch( embed reg[], size_t num, void *workspace ) throw()
		{
			assert( !(num>0 && NULL == reg) );
			uint8_t *addr = static_cast<uint8_t*> (workspace);
			for( size_t i=0; i < num; ++i )
			{
				embed *em = &reg[i];
				assert( NULL != em->hook );
				assert( NULL == *(em->hook) );
				assert( !(workspace==NULL && em->offset>0) );
				*(em->hook) = &addr[ em->offset ];
			}
		}
		
		void  *embed:: build_with( allocator &alloc, embed reg[], size_t num, size_t &bytes )
		{
			bytes      = prepare( reg, num );
			void *wksp = alloc.acquire( bytes );
			dispatch(reg,num,wksp);
			return wksp;
		}
		
		
	}
	
}

