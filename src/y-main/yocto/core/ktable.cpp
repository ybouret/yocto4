#include "yocto/core/ktable.hpp"
#include "yocto/code/primes.hpp"

#include "yocto/code/round.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/swap.hpp"

#include "yocto/exceptions.hpp"
#include <cerrno>

#include <iostream>

namespace yocto
{
	namespace core
	{
		ktable:: ~ktable() throw() 
		{
			assert( 0 == in_slots() );
		}
		
		ktable:: ktable() throw() :
		wksp_size_(0),
		wksp_addr_(NULL),
		slot_(NULL),
		nopt_(0),
		pool_()
		{
		}
		
		ktable:: ktable( size_t n, memory::allocator &alloc ) :
		wksp_size_(0),
		wksp_addr_(NULL),
		slot_(NULL),
		nopt_(0),
		pool_()
		{
			if( n > 0 )
			{
				//-- compute #node
				size_t nodes = max_of<size_t>(YOCTO_KTABLE_MIN_SLOTS * YOCTO_KTABLE_LOAD_FACTOR,n);
				while( 0 != ( nodes % YOCTO_KTABLE_LOAD_FACTOR ) )
				{
					++nodes;
					if( nodes < n ) throw libc::exception( ERANGE, "ktable nodes overflow@%u", unsigned(n) );
				}
				
				//-- deduce #slots
				nopt_ = next_prime( nodes/YOCTO_KTABLE_LOAD_FACTOR );
				
				//-- recompute #nodes
				nodes = nopt_ * YOCTO_KTABLE_LOAD_FACTOR;
				
				std::cerr << "n= " << n << " => nopt= " << nopt_ << " => " << nodes << " nodes: ";
				assert( nodes >= n );
				
				//-- compute memory_layout
				const size_t slots_length = nopt_ * sizeof(kslot);
				const size_t nodes_offset = YOCTO_MEMALIGN(slots_length);
				const size_t nodes_length = nodes * sizeof(knode);
				wksp_size_ = nodes_offset + nodes_length;
				std::cerr << "requiring " << wksp_size_ << " bytes...@0+" << slots_length << " | @" << nodes_offset << "+" << nodes_length << std::endl;
				
				//-- acquire memory
				try {
					wksp_addr_ = alloc.acquire( wksp_size_ );
				}
				catch (...) {
					assert( wksp_size_ == 0 );
					wksp_addr_ = NULL;
					nopt_      = 0;
				}
				
				//-- link memory
				uint8_t *p = static_cast<uint8_t *>( wksp_addr_ );
				slot_      = (kslot *)&p[0];
				knode  *kn = (knode *)&p[nodes_offset]; 
				for( size_t i=0; i < nodes; ++i )
				{
					pool_.store( &kn[i] );
				}
			}
		}
		
		
		void ktable:: release( memory::allocator &alloc ) throw()
		{
			alloc.release( wksp_addr_, wksp_size_ );
			slot_ = NULL;
			nopt_ = 0;
			pool_.reset();
		}
		
		void ktable:: swap_with( ktable &other ) throw()
		{
			cswap( wksp_size_, other.wksp_size_ );
			cswap( wksp_addr_, other.wksp_addr_ );
			cswap( slot_, other.slot_ );
			cswap( nopt_, other.nopt_ );
			mswap( pool_, other.pool_ );
		}
		
		size_t ktable:: available() const throw()
		{
			return pool_.size;
		}
		
		size_t ktable:: in_slots()  const throw()
		{
			size_t ans = 0;
			for( size_t i=0; i < nopt_; ++i )
			{
				ans += slot_[i].size;
			}
			return ans;
		}
		
		knode:: knode( size_t k, void * p ) throw() : next(NULL), prev(NULL), key(k), ptr(p)  {}
		
		void ktable:: insert( size_t key, void *ptr ) throw()
		{
			assert( available() > 0 );
			assert( slot_ != NULL );
			assert( nopt_ > 0 );
			knode *node = pool_.query();
			slot_[ key % nopt_ ].push_front( new (node) knode(key,ptr) );
		}
		
		void ktable:: insert2( kslot *slot, size_t key, void *ptr ) throw()
		{
			assert( available() > 0 );
			assert( slot_ != NULL );
			assert( nopt_ > 0 );
			assert( slot != NULL );
			assert( slot >= slot_ );
			assert( slot < slot_ + nopt_ );
			assert( slot_ + (key%nopt_) == slot );
			knode *node = pool_.query();
			slot->push_front( new (node) knode(key,ptr) );
		}
		
		knode * ktable:: search(size_t key, const void *target, bool (*proc)( const void *target, const void *ptr ), kslot * &slot ) throw()
		{
			assert(proc);
			assert(NULL==slot);
			if( nopt_ > 0 )
			{
				assert( slot_ );
				slot = &slot_[ key % nopt_ ];
				for( knode *node = slot->head; node; node=node->next )
				{
					assert( (node->key % nopt_) == ( key % nopt_ ) );
					if( proc( target, node->ptr ) )
					{
						slot->move_to_front(node);
						return node;
					}
				}
				return NULL;
			}
			else 
			{
				return NULL;
			}
			
		}
		
		void ktable:: remove_front_of( kslot *slot ) throw()
		{
			assert( slot_ != NULL );
			assert( nopt_ > 0 );
			assert( slot != NULL );
			assert( slot >= slot_ );
			assert( slot < slot_ + nopt_ );
			assert( slot->head != NULL   );
			
			pool_.store( slot->pop_front() );
			
			
		}
		
		void ktable:: move_to( ktable &other ) throw()
		{
			assert( in_slots() <= other.available() );
			for( size_t i=0; i < nopt_; ++i )
			{
				kslot *slot = &slot_[i];
				while( slot->size > 0 )
				{
					knode *node = slot->pop_back();
					other.insert( node->key, node->ptr );
					pool_.store(node);
				}
			}
		}

		
		
	}
	
	
}