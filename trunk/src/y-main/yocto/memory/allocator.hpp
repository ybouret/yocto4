#ifndef YOCTO_MEMORY_ALLOCATOR_INCLUDED
#define YOCTO_MEMORY_ALLOCATOR_INCLUDED 1

#include "yocto/threading/singleton.hpp"

namespace yocto 
{
	
	namespace memory 
	{
		//! allocator interface
		class allocator 
		{
		public:
			virtual ~allocator() throw();
			
			virtual void *acquire( size_t &n ) = 0;
			virtual void  release( void * &p, size_t &n ) throw() = 0;
			
			template <typename T>
			T *acquire_as( size_t &n )
			{
				size_t bytes = n * sizeof(T);
				try
				{
					T     *ptr  = static_cast<T*>( acquire(bytes) );
					assert( bytes >= n * sizeof(T) );
					n = bytes / sizeof(T);
					return ptr;
				}
				catch(...)
				{
					assert( bytes == 0 );
					n = 0;
					throw;
				}
			}
			
			template <typename T>
			void release_as( T * &p, size_t &n ) throw()
			{
				size_t bytes = n * sizeof(T);
				release( *(void **) &p, bytes );
				assert( 0 == bytes );
				n = 0;
			}
			
			
			
		protected:
			explicit allocator() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(allocator);
		};
		
		//! allocator singleton wrapper
		template <typename ALLOCATOR>
		struct kind
		{
			static void *acquire( size_t &n ) 
			{ 
				static ALLOCATOR &h = ALLOCATOR::instance(); 
				return h.acquire(n);
			}
			
			static void  release( void *&p, size_t &n) throw()
			{
				static ALLOCATOR &h = ALLOCATOR::instance(); 
				return h.release(p,n);
			}
			
			template <typename T>
			static T * acquire_as( size_t &n )
			{
				static ALLOCATOR &h = ALLOCATOR::instance(); 
				return h.acquire_as<T>(n);
			}
			
			template <typename T>
			static void release_as( T * &p, size_t &n ) throw()
			{
				static ALLOCATOR &h = ALLOCATOR::instance(); 
				h.release_as<T>(p,n);
			}
		};
	}
	
}

#endif
