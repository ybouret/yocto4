#include "yocto/memory/pooled.hpp"


namespace yocto
{
	
	namespace memory
	{
		const char pooled::name[] = "memory::pooled";
		
		pooled:: ~pooled() throw() {}
		
		pooled:: pooled() throw() :
		carver_( YOCTO_CHUNK_SIZE/2, 16 ),
		mini_size( carver_.mini_size ),
		max_in_pool( carver_.max_in_pool )
		{
		}
	
		void * pooled:: acquire( size_t &n )
		{
			YOCTO_LOCK( access );
			return carver_.acquire( n );
		}
		
		void pooled:: release( void * &p, size_t &n ) throw()
		{
			YOCTO_LOCK( access );
			carver_.release(p);
			p = NULL;
			n = 0;
		}
		
		pooled:: allocator:: allocator() throw()
		{
			(void) pooled::instance();
		}
		
		pooled:: allocator:: ~allocator() throw()
		{
			
		}
		void * pooled:: allocator:: acquire( size_t &n )
		{
			static pooled &g = pooled::instance();
			return g.acquire( n );
		}
		
		void pooled:: allocator:: release( void * &p, size_t &n ) throw()
		{
			static pooled &g = pooled::instance();
			return g.release(p,n);
		}
		
		
	}
	
}
