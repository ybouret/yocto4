#ifndef YOCTO_CORE_KTABLE_INCLUDED
#define YOCTO_CORE_KTABLE_INCLUDED

#include "yocto/memory/allocator.hpp"
#include "yocto/core/list.hpp"
#include "yocto/core/pool.hpp"

namespace yocto
{
	
#ifndef YOCTO_KTABLE_LOAD_FACTOR
#define YOCTO_KTABLE_LOAD_FACTOR 4
#endif
	
#ifndef YOCTO_KTABLE_MIN_SLOTS
#define YOCTO_KTABLE_MIN_SLOTS   3
#endif
	
	namespace core
	{
		
		class ktable;
		
		class knode
		{
		public:
			knode       *next;
			knode       *prev;
			const size_t key;
			void        *ptr;
		private:
			~knode() throw();
			knode(size_t , void* ) throw();
			YOCTO_DISABLE_COPY_AND_ASSIGN(knode);
			friend class ktable;
		};
		
		typedef list_of<knode> kslot;
		typedef pool_of<knode> kpool;
		
		class ktable 
		{
		public:
			explicit ktable() throw();
			virtual ~ktable() throw();
			
			explicit ktable( size_t n, memory::allocator &alloc );
			void     release( memory::allocator &alloc ) throw();
			void     swap_with( ktable &other ) throw();
			size_t   available() const throw();
			size_t   in_slots()  const throw();
			
			//! insert a (key,ptr) value
			/**
			 *	must have available() > 0
			 */
			void     insert( size_t key, void *ptr ) throw(); 
			
			//! insert a (key,ptr) in a previously found slot
			/**
			 *	must have available() > 0
			 */
			void     insert2( kslot *slot, size_t key, void *ptr ) throw(); 
			
			//! generic search
			/**
			 *	for each node in slot = &slot_[ key%nopt_ ], return first proc(target,node->ptr) == true.
			 *	if found, node is moved to front of slot
			 */
			knode *search(size_t key, const void *target, bool (*proc)( const void *target, const void *ptr ), kslot * &slot ) throw();
			
			//! generic remove
			/**
			 * remove node from a previous search.
			 */
			void remove_front_of( kslot *slot ) throw();
			
			
			//! transfer ownership
			void move_to( ktable &other ) throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ktable);
			size_t wksp_size_;
			void  *wksp_addr_;
			
			kslot *slot_; //!< nodes sharing the same key % nopt_
			size_t nopt_; //!< a prime number
			kpool  pool_; //!< embbeded empty knodes
			
		};
		
	}
}

#endif
