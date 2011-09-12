#include "yocto/ocl/types.hpp"

namespace yocto
{

	namespace ocl
	{

		namespace Core
		{

			static inline void * __mem_acquire( size_t n )
			{
				assert(n>0);
				static BaseAllocator &mgr = BaseAllocator::instance();
				n = YOCTO_MEMALIGN(n);
				return mgr.acquire( n );
			}

			static inline void __mem_release( void *p, size_t n ) throw()
			{
				assert(p!=NULL);
				assert(n>0);
				n = YOCTO_MEMALIGN(n);
				static BaseAllocator &mgr = *( BaseAllocator::location() );
				mgr.release( p );
			}

			void * Object:: operator new( size_t block_size )
			{
				assert( block_size > 0 );
				return __mem_acquire( block_size );
			}

			void * Object:: operator new[] ( size_t block_size )
			{
				assert( block_size > 0 );
				return __mem_acquire( block_size );
			}

			void Object:: operator delete( void *p, size_t block_size )
			{
				assert( block_size > 0 );
				if(p) __mem_release( p, block_size );
			}

			void Object:: operator delete[]( void *p, size_t block_size )
			{
				assert( block_size > 0 );
				if(p!=NULL)
					__mem_release( p, block_size );
			}


			Object:: Object() throw() : ReferenceCount(0)
			{

			}

			Object:: ~Object() throw()
			{
				assert( 0 == ReferenceCount );
			}

			void Object:: Retain() throw()
			{
				++( *(size_t*) &ReferenceCount );
			}

			bool Object:: Release() throw()
			{
				assert( ReferenceCount> 0 );
				return (--( *(size_t*) &ReferenceCount )) <= 0;

			}


		}
	}

}
