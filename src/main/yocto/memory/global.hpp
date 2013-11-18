#ifndef YOCTO_MEMORY_GLOBAL_INCLUDED
#define YOCTO_MEMORY_GLOBAL_INCLUDED 1

#include "yocto/threading/singleton.hpp"
#include "yocto/memory/allocator.hpp"
#include "yocto/type/ints.hpp"

namespace yocto
{
	namespace memory
	{
		
		class global : public singleton<global>, public allocator
		{
		public:
			
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
			
			static int64_t allocated() throw();
			
			virtual void *acquire( size_t &n );
			virtual void  release( void * &p, size_t &n ) throw();
			
			//! simple wrappers for third party libs
			static void * __calloc( size_t count, size_t size );
			static void   __free( void * ) throw();
			
		private:
			explicit global() throw();
			virtual ~global() throw();
			YOCTO_DISABLE_COPY_AND_ASSIGN(global);
			friend class singleton<global>;
			
		public:
			static const char name[];
			static const threading::longevity life_time = limit_of<threading::longevity>::maximum;
		};
		
		
	}
}

#endif
