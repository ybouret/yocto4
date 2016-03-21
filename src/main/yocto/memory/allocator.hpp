#ifndef YOCTO_MEMORY_ALLOCATOR_INCLUDED
#define YOCTO_MEMORY_ALLOCATOR_INCLUDED 1

#include "yocto/threading/singleton.hpp"
#include <cstdarg>

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
				return h.template acquire_as<T>(n);
			}
			
			template <typename T>
			static void release_as( T * &p, size_t &n ) throw()
			{
				static ALLOCATOR &h = ALLOCATOR::instance(); 
				h.template release_as<T>(p,n);
			}

            //! acquire multiple chunks
            /**
             T *p,*q;
             size_t n=100;
             acquire_chunks<T>(n,2,&p,&q);
             release_as<T>(p,n); q=0; ...
             */
            template <typename T>
            static void acquire_chunks(size_t &n,const size_t num,...)
            {
                assert(num>0);
                if(n>0)
                {
                    //__________________________________________________________
                    //
                    // allocate memory
                    //__________________________________________________________
                    const size_t chunk_size = n;
                    n*=num;
                    T *addr = acquire_as<T>(n);

                    //__________________________________________________________
                    //
                    // shift assign
                    //__________________________________________________________
                    va_list ap;
                    va_start(ap,num);
                    for(size_t i=1;i<=num;++i)
                    {
                        T **ppChunk = va_arg(ap,T**);
                        assert(ppChunk);
                        *ppChunk = addr;
                        addr += chunk_size;
                    }
                    va_end(ap);
                }
                else
                {
                    //__________________________________________________________
                    //
                    // no throw set everyone to NULL
                    //__________________________________________________________
                    va_list ap;
                    va_start(ap,num);
                    for(size_t i=1;i<=num;++i)
                    {
                        T **ppChunk = va_arg(ap,T**);
                        assert(ppChunk);
                        *ppChunk = 0;
                    }
                    va_end(ap);
                }
            }
		};
	}
	
}

#endif
