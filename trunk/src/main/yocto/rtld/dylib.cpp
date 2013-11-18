#include "yocto/rtld/dylib.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/error.hpp"
#include "yocto/exceptions.hpp"

#if defined(YOCTO_BSD)
#include <dlfcn.h>
#endif

#include <cstring>

#include <iostream>

namespace yocto
{
    
    struct dylib
    {
        void  *h; //!< handle
        size_t n; //!< #ref
		static inline dylib *acquire() { return object::acquire1<dylib>(); }
		static inline void   release(dylib *dll) throw() { assert(dll); memset(dll,0,sizeof(dylib)); object::release1<dylib>(dll); }
        
#if defined(YOCTO_BSD)
        static inline int flag2args( dylib_flag flag )
        {
            switch( flag )
            {
                case dylib_full:
                    return RTLD_NOW;
                    
                case dylib_lazy:
                    return RTLD_LAZY;
            }
            throw exception("invalid dlopen flag");
        }
#endif
        
#if defined(YOCTO_WIN)
        static inline void *handle_of( const char *soname, dylib_flag flag )
        {
            switch(flag)
            {
                case dylib_full: return ::LoadLibrary(soname);
                case dylib_lazy: return ::LoadLibraryEx(soname,0,DONT_RESOLVE_DLL_REFERENCES);
                default:
                    throw exception("invalid LoadLibrary flag");
            }

        }
#endif
        
    };
	
	
    
	dylib *dylib_load( const char *soname , dylib_flag flag)
	{
		dylib *dll = dylib::acquire();
		YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
        dll->h = dlopen(soname, dylib::flag2args(flag) );
		if(0==dll->h)
		{
			dylib::release(dll);
			throw imported::exception( "dlopen", "%s", dlerror() );
		}
		return dll;
#endif
		
#if defined(YOCTO_WIN)
        dll->h = dylib::handle_of(soname,flag);
        if(0==dll->h)
		{
            const DWORD err = ::GetLastError();
			dylib::release(dll);
			throw win32::exception( err, "::LoadLibrary");
		}
		return dll;
#endif
	}
	
    dylib *dylib_load( const char *soname, dylib_flag flag, char *errbuf, size_t errlen)
    {
        
        assert(!(0==errbuf&&errlen>0));
        
        dylib *dll = dylib::acquire();
        memset(errbuf,0,errlen);
        YOCTO_GIANT_LOCK();
        
#if defined(YOCTO_BSD)
        dll->h = dlopen(soname, dylib::flag2args(flag) );
        if(0==dll->h)
        {
            if(errbuf)
            {
                assert(errlen>0);
                const char  *dl_err = dlerror();
                const size_t dl_len = length_of( dl_err );
                memcpy(errbuf,dl_err,min_of<size_t>(dl_len,errlen-1));
            }
			dylib::release(dll);
            return 0;
        }
#endif
        
#if defined(YOCTO_WIN)
        dll->h = dylib::handle_of(soname,flag);
		if(0==dll->h)
		{
			if(errbuf)
			{
				win32::format_error(errbuf,errlen, ::GetLastError() );
			}
			dylib::release(dll);
			return 0;
		}
#endif
        assert(dll->h);
        return dll;
    }
    
#define Y_DLL_CHECK() assert(0!=dll); assert(0!=dll->h)
    void dylib_incr(dylib *dll) throw()
	{
		Y_DLL_CHECK();
		++(dll->n);
	}
	
	size_t dylib_decr(dylib *dll) throw()
	{
		Y_DLL_CHECK();
		assert(dll->n>0);
		return --(dll->n);
	}
	
	
    void dylib_free( dylib *dll ) throw()
    {
		Y_DLL_CHECK();
		assert(0==dll->n);
		assert(dll->h);
        
#if defined(YOCTO_BSD)
		dlclose(dll->h);
#endif
        
#if defined(YOCTO_WIN)
		:: FreeLibrary((HINSTANCE)(dll->h));
#endif
		dylib::release(dll);
    }
    
	
	void *dylib_addr( const dylib *dll,  const char *symbol ) throw()
	{
		Y_DLL_CHECK();
		if(symbol)
		{
#if defined(YOCTO_BSD)
			return dlsym( (void*)(dll->h), symbol );
#endif
			
#if defined(YOCTO_WIN)
			return (void*) ::GetProcAddress( (HMODULE)(dll->h), symbol );
#endif
		}
		return 0;
	}
}
