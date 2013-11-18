#ifndef YOCTO_THREADING_SINGLETON_INCLUDED
#define YOCTO_THREADING_SINGLETON_INCLUDED 1

#include "yocto/threading/mutex.hpp"
#include "yocto/threading/at-exit.hpp"
#include "yocto/code/round.hpp"
#include <new>

namespace yocto
{
	namespace hidden
    {
        void singleton_out( const char *name, const char *mesg, const int life_time) throw();
    }
    
    //! global flag for singleton creation
    /**
     Used at the first call of instance(). Default if 'false'.
     */
    extern bool singleton_verbosity;

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
		static bool             verbose;
        
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
						verbose = singleton_verbosity;
                        if(verbose) hidden::singleton_out( T::name, "registering", T::life_time);
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
		
		static const char * get_name() throw()
		{
			return T::name;
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
                if(verbose) hidden::singleton_out( T::name, "destroying", T::life_time);
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
	
    template <typename T>
    bool singleton<T>::verbose = false;
    
	
}


#endif
