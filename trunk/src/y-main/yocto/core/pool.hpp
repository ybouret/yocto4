#ifndef YOCTO_CORE_POOL_INCLUDED
#define YOCTO_CORE_POOL_INCLUDED 1


#include "yocto/os.hpp"

namespace yocto 
{
	namespace core
	{
		//! core pool
		template <typename NODE>
		class pool_of
		{
		public:
			//! default constructor
			inline explicit pool_of() throw() : top(NULL), size(0) {}
			
			//! destructor: pool must be empty
			inline virtual ~pool_of() throw() { assert(NULL==top); assert(0==size); }
			
			NODE  *top;  //!< top of pool
			size_t size; //!< nodes in pool
			
						
			//! push a valid node
			inline void push( NODE *node ) throw()
			{
				assert( NULL != node       );
				assert( NULL == node->next );
				assert( ! owns(node)       );
				node->next = top;
				top        = node;
				++size;
			}
			
			//! pop is size>0
			inline NODE *pop() throw()
			{
				assert(top != NULL);
				assert(size > 0   );
				NODE *node = top;
				top = top->next;
				node->next = NULL;
				--size;
				return node;
			}
			
			//! linear search
			inline bool owns( const NODE *node ) const throw()
			{
				for( const NODE *scan = top; scan != NULL; scan = scan->next )
				{
					if( scan == node ) return true;
				}
				return false;
			}
			
			inline void delete_with( void (*proc)( NODE *, void *), void *args ) throw()
			{
				assert(proc);
				while(size>0) proc( pop(), args ); 
			}
			
			inline void delete_with( void (*proc)(NODE *) ) throw()
			{
				assert( proc );
				while(size>0) proc( pop() );
			}
			
			inline void reset() throw() { top = NULL; size = 0; }

			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(pool_of);
		};
		
		
	}
	
}

#endif
