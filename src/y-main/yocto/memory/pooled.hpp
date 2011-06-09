#ifndef YOCTO_MEMORY_POOLED_INCLUDED
#define YOCTO_MEMORY_POOLED_INCLUDED 1

#include "yocto/memory/carver.hpp"
#include "yocto/object.hpp"


namespace yocto
{
	
	namespace memory
	{
	
		class pooled : public singleton<pooled>, public allocator
		{
		public:			
			virtual void *acquire( size_t &n );
			virtual void  release( void * &p, size_t &n ) throw();
			
			//! light weight singleton wrapper
			class allocator : public memory::allocator
			{
			public:
				explicit allocator() throw();
				virtual ~allocator() throw();
				
				virtual void * acquire( size_t &n );
				virtual void   release( void * &p, size_t &n ) throw();
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(allocator);
			};
			
			
		private:
			explicit pooled() throw();
			virtual ~pooled() throw();
			carver carver_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(pooled);
			static const char name[];
			friend class singleton<pooled>;

		public:			
			size_t & mini_size;
			size_t & max_in_pool;
			
			static const threading::longevity life_time = object::provider::life_time-1;

		};
		
		
	}
	
}

#endif
