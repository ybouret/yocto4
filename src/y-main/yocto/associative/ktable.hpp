#ifndef YOCTO_ASSOCIATIVE_KTABLE_INCLUDED
#define YOCTO_ASSOCIATIVE_KTABLE_INCLUDED 1

#include "yocto/memory/slab.hpp"
#include "yocto/core/list.hpp"
#include "yocto/memory/embed.hpp"
#include "yocto/code/swap.hpp"

#include "yocto/hashing/sha1.hpp"

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
		/** Node must have a hkey field */
		template <typename NODE>
		class knode
		{
		public:
			knode       *next;
			knode       *prev;
			NODE        *addr;
			
			inline knode( NODE *n ) throw() : next(NULL), prev(NULL), addr(n) {}
			
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
					{ (void**)(void *) & kslot, count * sizeof(kslot_t),      0 },
					{  & kpool_entry,    kslab_t::bytes_for(nodes),    0 },
					{  & cache_entry,    cache_t::bytes_for(nodes),    0 }
				};
				
				//--------------------------------------------------------------
				// get the memory
				//--------------------------------------------------------------
				buflen_ = memory::embed::prepare( reg, 3 );
				try { buffer_ = alloc.acquire( buflen_ ); }
				catch(...) { nodes = 0; count=0; throw; }
				memory::embed::dispatch( reg, 3, buffer_ );
				
				//--------------------------------------------------------------
				// finish up
				//--------------------------------------------------------------
				kslab_t tmp_kpool( kpool_entry, nodes ); mswap( kpool, tmp_kpool );
				cache_t tmp_cache( cache_entry, nodes ); mswap( cache, tmp_cache );
			}
			
			inline size_t num_bytes() const throw() { return buflen_; }
			
			inline void release_all( memory::allocator &alloc ) throw()
			{
				assert( 0 == kpool.involved() ); assert( 0 == cache.involved() );
				alloc.release( buffer_, buflen_ );
				nodes = 0;
				count = 0;
				kslot = NULL;
				kslab_t tmp_kpool(NULL,0); mswap( kpool, tmp_kpool );
				cache_t tmp_cache(NULL,0); mswap( cache, tmp_cache );
			}
			
			inline size_t in_slots() const throw()
			{
				size_t num = 0; 
				for( size_t i=0; i < count; ++i ) num += kslot[i].size; 
				return num;
			}
			
			//! insert a node extracted from cache
			inline void insert( NODE *node ) throw()
			{
				assert( kpool.available() > 0 ); assert( count > 0 );
				knode_t *kn = new (kpool.query()) knode_t( node );
				kslot[ node->hkey % count ].push_front( kn );
				nlist.push_back( node );
			}
			
			
			//! free
			inline void free_with( void (*proc)( NODE * ) throw() ) throw()
			{
				assert(proc);
				for( size_t i=0; i < count; ++i )
				{
					kslot_t &slot = kslot[i];
					while( slot.size > 0 )
					{
						//-- remove knode from slot
						knode_t *kn = slot.pop_front(); 
						assert( kn->addr != NULL );
						assert( kn->addr->hkey % count == i );
						
						//-- remove NODE from node list
						NODE    *dn = nlist.unlink(kn->addr);
						
						//-- cleanup
						proc( dn );
						kn->addr = NULL;
						
						//-- store back
						cache.store( dn );
						kpool.store( kn );
					}
				}
			}
			
			
			//! search
			inline NODE *search( size_t hkey, kslot_t * &slot, bool (*match)(const NODE *, const void *param), const void *param ) throw()
			{
				assert( NULL == slot );
				if(count>0)
				{
					const size_t indx = hkey % count;
					slot = &kslot[ indx ];
					for( knode_t *kn = slot->head; kn != NULL; kn = kn->next )
					{
						NODE *node = kn->addr; assert(node); assert(node->hkey % count == indx );
						if( match(node,param) ) 
						{
							slot->move_to_front(kn);
							return node;
						}
					}
					return NULL;
				}
				else return NULL;
			}
			
			//! insert at a slot found by a previous search
			inline void insert2( NODE *node, kslot_t *slot ) throw()
			{
				assert( kpool.available() > 0 ); assert( count > 0 );
				assert( node != NULL ); assert( slot != NULL );
				assert( node->hkey % count == static_cast<size_t> (slot-kslot) );
				knode_t *kn = new (kpool.query()) knode_t( node );
				slot->push_front( kn  );
				nlist.push_back( node );
			}
			
			//! remove the front node from a previous search
			inline void remove_front_of( kslot_t *slot, void (*proc)( NODE * ) throw() ) throw()
			{
				assert(slot); assert(slot->size>0);
				assert(proc);
				knode_t *kn  = slot->pop_front(); assert( kn->addr != NULL );
				NODE    *dn  = nlist.unlink( kn->addr );
				proc(dn);
				kpool.store(kn);
				cache.store(dn);
			}
			
			//! get all hash
			size_t signature() const throw()
			{
				hashing::sha1 H;
				H.set();
				for( const NODE *dn = nlist.head; dn; dn = dn->next )
					H( & dn->hkey, sizeof(size_t) );
				return H.key<size_t>();
			}
			
		private:
			void  *buffer_;
			size_t buflen_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(ktable);
		};
	}
}


#endif
