#ifndef YOCTO_ASSOCIATIVE_KTABLE_INCLUDED
#define YOCTO_ASSOCIATIVE_KTABLE_INCLUDED 1

#include "yocto/type-traits.hpp"
#include "yocto/memory/slab.hpp"
#include "yocto/core/list.hpp"
#include "yocto/memory/embed.hpp"
#include "yocto/code/swap.hpp"

namespace yocto
{
	
	namespace core
	{
		
#ifndef YOCTO_KTABLE_LOAD_FACTOR
#define YOCTO_KTABLE_LOAD_FACTOR 4
#endif
		
#ifndef YOCTO_KTABLE_MIN_SLOTS
#define YOCTO_KTABLE_MIN_SLOTS   3
#endif
		
		
		//! key meta-node
		template <typename NODE>
		class knode
		{
		public:
			knode       *next;
			knode       *prev;
			const size_t hkey;
			NODE        *addr;
			
			inline knode( size_t k, NODE *n ) throw() : hkey(k), addr(n) {}
		private:
			~knode() throw();
			YOCTO_DISABLE_COPY_AND_ASSIGN(knode);
		};
		
		size_t ktable_num_slots_for( size_t &num_items );
		
		template <typename NODE>
		class ktable
		{
		public:
			typedef knode<NODE>              knode_t;
			typedef list_of<knode_t>         kslot_t;
			typedef memory::slab_of<knode_t> kslab_t;
			typedef memory::slab_of<NODE>    cache_t;
			typedef list_of<NODE>            nlist_t;
			
			kslot_t  *kslot; //!< linear slots[0..count-1]
			size_t    count; //!< #kslot
			size_t    nodes; //!< max #nodes
			nlist_t   nlist; //!< NODE list
			kslab_t   kpool; //!< knodes provider
			cache_t   cache; //!< NODEs provider
			
			inline  ktable() throw() : kslot(NULL), count(0), nodes(0), nlist(), kpool(NULL,0), cache(NULL,0), buffer_(NULL), buflen_(0) {}
			inline ~ktable() throw() { assert( 0 == kpool.involved() ); assert( 0 == cache.involved() ); }
			
			inline  ktable( size_t num_items, memory::allocator &alloc ) :  kslot(NULL), count(0), nodes(num_items), nlist(), kpool(NULL,0), cache(NULL,0), buffer_(NULL), buflen_(0) 
			{
				//--------------------------------------------------------------
				// compute memory requirement
				//--------------------------------------------------------------
				count = ktable_num_slots_for(nodes);
				void *kpool_entry = NULL;
				void *cache_entry = NULL;
				memory::embed reg[] = 
				{
					{ (void **) & kslot, count * sizeof(kslot_t),      0 },
					{  & kpool_entry,    kslab_t::bytes_for(nodes),    0 },
					{  & cache_entry,    cache_t::bytes_for(nodes),    0 }
				};
				
				//--------------------------------------------------------------
				// get the memory
				//--------------------------------------------------------------
				buflen_ = memory::embed::prepare( reg, 3 );
				try { buffer_ = alloc.acquire( buflen_ ); }
				catch(...) { nodes = 0; throw; }
				memory::embed::dispatch( reg, 3, buffer_ );
				
				//--------------------------------------------------------------
				// finish up
				//--------------------------------------------------------------
				kslab_t tmp_kpool( kpool_entry, nodes ); mswap( kpool, tmp_kpool );
				cache_t tmp_cache( cache_entry, nodes ); mswap( cache, tmp_cache );
			}
			
			inline size_t num_bytes() const throw() { return buflen_; }
			
		private:
			void  *buffer_;
			size_t buflen_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(ktable);
		};
	}
}


#endif
