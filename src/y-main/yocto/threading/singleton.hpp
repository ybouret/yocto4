#ifndef YOCTO_THREADING_SINGLETON_INCLUDED
#define YOCTO_THREADING_SINGLETON_INCLUDED 1

#include "yocto/threading/mutex.hpp"
#include "yocto/threading/at-exit.hpp"
#include "yocto/code/round.hpp"
namespace yocto
{
	
	//! singleton of T
	/**
	 T must define:
	 - a static char      name;
	 - a static longevity life_time
	 */
	template <typename T>
	class singleton
	{
	public:
		static threading::mutex access;
		
		static T & instance()
		{
			static volatile bool     register_ = true;
			static volatile uint64_t location_[ YOCTO_U64_FOR_ITEM(T) ];
			//------------------------------------------------------------------
			// Double Check Pattern
			//------------------------------------------------------------------
			YOCTO_LOCK(access);
			if ( !instance_ )
			{
				YOCTO_LOCK(access);
				if ( !instance_ ) 
				{
					//----------------------------------------------------------
					// once in a lifetime registering
					//----------------------------------------------------------
					if( register_ ) 
					{
						clear_( (void *)location_ );
						threading::at_exit::perform( release_, T::life_time);
						register_ = false;
					}
					
					//----------------------------------------------------------
					// access protected static memory
					//----------------------------------------------------------
					void     *instance_addr = (void *) &location_[0];
					instance_ = static_cast<volatile T *>( instance_addr );
					try
					{
						new ( instance_addr ) T();
					}
					catch(...)
					{
						instance_ = NULL;
						clear_( (void*)location_ );
						throw;
					}
				}
			}
			return *(T *)instance_;
		}
		
		static inline  T *location() throw()
		{
			assert( instance_ != NULL );
			return (T *)instance_;
		}
		
		static inline bool exists() throw()
		{
			return instance_ != NULL;
		}
		
	protected:
		inline explicit singleton() throw() {}
		inline virtual ~singleton() throw() {}
		
		//! allows early release
		static inline void destroy()
		{
			//-- double lock pattern
			YOCTO_LOCK(access);
			if( instance_ != NULL )
			{
				YOCTO_LOCK(access);
				release_();
			}
		}
		
	private:
		static volatile T *instance_;
		static inline void clear_( void *instance_addr ) throw() 
		{
			uint8_t *q = static_cast<uint8_t*>(instance_addr);
			for( size_t i=0;i<sizeof(T);++i) q[i] = 0;
		}
		YOCTO_DISABLE_COPY_AND_ASSIGN(singleton);
		
		//! called at exit
		static void release_() throw()
		{
			if (instance_ != NULL)
			{
				instance_->~T();
				clear_( (void *)instance_ );
				instance_ = NULL;
			}
		}
	};
	
	template <typename T>
	volatile T * singleton<T>::instance_ = NULL;
	
	template <typename T>
	threading::mutex singleton<T>::access( T::name );
	
	
}


#endif
