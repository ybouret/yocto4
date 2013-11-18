#include "yocto/threading/mutex.hpp"
#include "yocto/error.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/static-check.hpp"

#include <cstring>

#define YOCTO_MUTEX_VERBOSE 0

#if YOCTO_MUTEX_VERBOSE == 1
#include <iostream>
#endif

namespace yocto 
{
	namespace threading
	{
		
		const char * mutex:: name() const throw()
		{
			return name_;
		}
		
		mutex mutex::giant("GIANT_MUTEX");
	
		void mutex:: clear() throw()
		{
			//-- clean everything
			memset( &mutex_,  0, sizeof(mutex_)  );
			memset( name_,    0, sizeof(name_)   );
		}

		mutex:: ~mutex() throw()
		{
#if YOCTO_MUTEX_VERBOSE == 1
			std::cerr << "[-mutex] '" << name_ << "'" << std::endl;
#endif	
			
#if defined(YOCTO_WIN)
			::DeleteCriticalSection( & mutex_ );
#endif
			
#if defined(YOCTO_BSD)
			{
				const int res = pthread_mutex_destroy( &mutex_ );
				if( res != 0 )
					libc::critical_error( res, "pthread_mutex_destroy" );
			}
#endif
			clear();
		}
		
#if defined(_MSC_VER)
#pragma warning (disable:4351)
#endif
		mutex:: mutex( const char *id ) throw() :
		mutex_(),
		name_()
		{
			clear();

			//-- initialize the mutex
#if defined(YOCTO_WIN)
			::InitializeCriticalSection( & mutex_ );
#endif
			
#if defined(YOCTO_BSD)
			{
				pthread_mutexattr_t attr;
				
				int  res = pthread_mutexattr_init( &attr );
				if (res != 0)
					libc::critical_error(res, "pthread_mutexattr_init");
				
				res = pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE);
				if (res != 0)
				{
					pthread_mutexattr_destroy( &attr );
					libc::critical_error(res, "pthread_mutexattr_settype(RECURSIVE)");
				}
				
				res = pthread_mutex_init( & mutex_ , &attr );
				(void) pthread_mutexattr_destroy( &attr );
				if( res != 0 )
					libc::critical_error(res,"pthread_mutex_init");
			}
#endif
			
			//-- set the name
			if( id )
			{
				const size_t len = min_of<size_t>( strlen(id), sizeof(name_)-1);
				memcpy( name_, id, len );
			}
			else 
			{
				YOCTO_STATIC_CHECK(sizeof(name_)>2*sizeof(void*),mutex_name_too_small);
				union  {
					void   *addr;
					uint8_t data[sizeof(void*)]; 
				} alias = { this };
				for( size_t i=0,j=0; i < sizeof(void*); ++i )
				{
					const uint8_t B = alias.data[i];
					name_[j++] = hexa_char[ (B>>4) & 0xf ];
					name_[j++] = hexa_char[ (B   ) & 0xf ];
				}
			}
			
#if YOCTO_MUTEX_VERBOSE == 1
			std::cerr << "[+mutex] '" << name_ << "'" << std::endl;
#endif
			
		}
		
		void mutex::lock() throw()
		{
#if YOCTO_MUTEX_VERBOSE == 1
			std::cerr << "[@mutex] '" << name_ << "' : LOCK" << std::endl;
#endif
			
#if defined(YOCTO_WIN)
			::EnterCriticalSection(  & mutex_  );
#endif
			
#if defined(YOCTO_BSD)
			const int res = pthread_mutex_lock( &mutex_ );
			if( res != 0 )
				libc::critical_error( res,  "pthread_mutex_lock" );
#endif
			
		}
		
		
		void mutex:: unlock() throw()
		{
#if YOCTO_MUTEX_VERBOSE == 1
			std::cerr << "[@mutex] '" << name_ << "' : UNLOCK" << std::endl;
#endif
			
#if defined(YOCTO_WIN)
			::LeaveCriticalSection( &mutex_  );
#endif
			
#if defined(YOCTO_BSD)
			const int res = pthread_mutex_unlock( & mutex_ );
			if( res != 0 )
				libc::critical_error( res,  "pthread_mutex_lock" );
#endif
		}
		
		bool mutex:: try_lock() throw()
		{
#if YOCTO_MUTEX_VERBOSE == 1
			std::cerr << "[@mutex] '" << name_ << "' : TRY_LOCK" << std::endl;
#endif
			
#if defined(YOCTO_WIN)
			return TryEnterCriticalSection( &mutex_ ) == TRUE;
#endif
			
#if defined(YOCTO_BSD)
			return pthread_mutex_trylock( &mutex_ ) == 0;
#endif
		}
		
	}
	
}
