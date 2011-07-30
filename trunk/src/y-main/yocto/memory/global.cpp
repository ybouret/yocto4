#include "yocto/memory/global.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>
#include <cstdlib>

#if !defined(NDEBUG)
#include <iostream>
#endif

#include <cstring>

namespace yocto
{
	
	namespace memory
	{
		
		const char global::name[] = "memory::global";
		
#if !defined(NDEBUG)
		static int64_t __allocated__ = 0;
#endif
		
		global::  global() throw() {}
		
		global:: ~global() throw()
		{
#if !defined(NDEBUG)
			if( __allocated__ != 0 )
			{
				std::cerr << "[memory::global] still allocated=" << __allocated__ << std::endl;
			}
#endif
		}
		
		int64_t global:: allocated() throw()
		{
#if !defined(NDEBUG)
			return __allocated__;
#else
			return 0;
#endif
		}
		
		
		
		void * global:: acquire( size_t &n )
		{
			YOCTO_LOCK(access);
			if( n > 0 )
			{
				try 
				{
					void *ptr =  operator new( n );
					if( !ptr )
					{
						throw libc::exception( ENOMEM, "global::acquire(%u)", unsigned(n) );
					}
#if !defined(NDEBUG)
					__allocated__ += int64_t(n);
#endif
					memset( ptr, 0, n );
					return ptr;
				}
				catch(...)
				{
					n = 0;
					throw;
				}
			}
			else
			{
				return NULL;
			}
			
		}
		
		void global:: release( void * &p, size_t &n ) throw()
		{
			YOCTO_LOCK(access);
			if( n > 0 )
			{
				assert( p != NULL );
				operator delete(p);
#if !defined(NDEBUG)
				__allocated__ -= int64_t(n);
#endif
				p = NULL;
				n = 0;
			}
#if !defined(NDEBUG)
			else 
			{
				assert( NULL == p );
			}
			
#endif
			
		}
		
		global:: allocator:: allocator() throw()
		{
			(void) global::instance();
		}
		
		global:: allocator:: ~allocator() throw()
		{
			
		}
		void * global:: allocator:: acquire( size_t &n )
		{
			static global &g = global::instance();
			return g.acquire( n );
		}
		
		void global:: allocator:: release( void * &p, size_t &n ) throw()
		{
			static global &g = global::instance();
			return g.release(p,n);
		}
		
		void * global:: __calloc( size_t count, size_t size )
		{
			
			assert( size > 0 );
			if( count > 0 )
			{
				YOCTO_GIANT_LOCK();
				void *p = calloc( count, size );
				if( !p )
					throw libc::exception( ENOMEM, "calloc(%u,%u)", unsigned(count), unsigned(size) );
				return p;
			}
			else
				return NULL;
		}
		
		void global::__free( void *p ) throw()
		{
			if( p )
			{
				YOCTO_GIANT_LOCK();
				free(p);
			}
		}

		
		
	}
	
}
