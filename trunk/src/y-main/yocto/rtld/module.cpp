#include "yocto/rtld/module.hpp"
#include "yocto/exceptions.hpp"

#if defined(YOCTO_BSD)
#include <dlfcn.h>
#endif

namespace yocto
{
	class module::impl  : public object
	{
	public:
		static inline void *load_library( const char *soname )
		{
			YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
			void *h = dlopen( soname, RTLD_NOW );
			if( !h )
				throw imported::exception( "dlopen", "%s", dlerror() );
			return h;
#endif
			
#if defined(YOCTO_WIN)
			void *h = ::LoadLibrary( soname );
			if( !h )
				throw win32:: exception( ::GetLastError(), "::LoadLibrary(%s)" );
			return h;
#endif
		}
		
		explicit impl( const string &soname ) :
		handle( load_library( soname.c_str() ) ),
		count(1)
		{
			
		}
		
		virtual ~impl() throw() 
		{
			YOCTO_GIANT_LOCK();
			assert( 0    == count  );
			assert( NULL != handle );
			
#if defined(YOCTO_BSD)
			dlclose( handle );
#endif
			
#if defined(YOCTO_WIN)
			::FreeLibrary( handle );
#endif
			handle = NULL;
		}
		
		void   *handle;
		size_t  count;
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(impl);
	};
	
	module:: module( const string &soname ) : impl_( new impl(soname) )
	{
	}
	
	module:: module( const module &other ) throw() : impl_( other.impl_ )
	{
		++( impl_->count );
	}
	
	module:: ~module() throw()
	{
		assert( impl_ );
		assert( impl_->count > 0 );
		if( --(impl_->count) <= 0 )
		{
			delete impl_;
		}
		impl_ = NULL;
	}
	
	
	void * module:: query( const string &symbol ) throw()
	{
#if defined(YOCTO_BSD)
		return dlsym( (impl_->handle), symbol.c_str() );
#endif
		
#if defined(YOCTO_WIN)
		return ::GetProcAddress( (HMODULE)(impl_->handle), symbol.c_str() );
#endif
	}
	
	void * module:: query( const char *symbol )
	{
		const string s = symbol;
		return query( s );
	}
	
	
}


