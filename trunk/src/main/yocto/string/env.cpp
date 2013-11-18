#include "yocto/string/env.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>
#include <cstring>

#if defined(YOCTO_BSD)
#include <cstdlib>
#if defined(__INTEL_COMPILER)
#pragma warning ( disable : 1419 )
#endif
extern char **environ;
#endif

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "yocto/memory/buffers.hpp"
#endif


namespace yocto {
	
	using namespace memory;
	
	namespace
	{
		
		class envmgr : public singleton<envmgr>
		{
		public:
			
			inline bool query( string &value, const string &name ) const
			{
				YOCTO_LOCK(access);
				
#if defined(YOCTO_BSD)
				const char *res = getenv( &name[0] );
				if( res ) {
					value = res;
					return true;
				} else {
					value.clear();
					return false;
				}
#endif
				
#if defined(YOCTO_WIN)
				//std::cerr << "GetEnvVar '" << name << "'" << std::endl;
				value.clear();
				const DWORD res = ::GetEnvironmentVariable( &name[0], NULL, 0);
				const DWORD err = ::GetLastError();
				if( err == ERROR_ENVVAR_NOT_FOUND )
				{
					//std::cerr << "Not Defined" << std::endl;
					return false;
				}
				else
				{
					//std::cerr << "[res1] = " << unsigned(res) << std::endl;
					const size_t           len    = res-1;
					const size_t           cnt    = res;
					buffer_of<char,global> blk(cnt);
					char                  *buf    = blk();
					const DWORD           res2    = ::GetEnvironmentVariable( &name[0], buf, res);
					//std::cerr << "[res2] = " << unsigned(res2) << std::endl;
					if(  res2 != len )
					{
						throw win32::exception( ::GetLastError(), "::GetEnvironmentVariable");
					}
					value.assign( buf, len );
					//std::cerr << "='" << value << "'" << std::endl;
					return true;
				}
#endif
				
			}
			
			
			inline void store( const string &name, const string &value ) const
			{
				YOCTO_LOCK(access);
#if defined(YOCTO_BSD)
				if( setenv( &name[0], &value[0], 1) < 0 ) {
					throw libc::exception( errno, "setenv");
				}
#endif
				
#if defined(YOCTO_WIN)
				YOCTO_GIANT_LOCK();
				if( ! ::SetEnvironmentVariable( &name[0], &value[0] ) ) {
					throw win32::exception( ::GetLastError(),  "::SetEnvironmentVariable");
				}
#endif
			}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(envmgr);
			friend class singleton<envmgr>;
			
			explicit envmgr() throw()
			{
			}
			
			virtual ~envmgr() throw()
			{
				
			}
			
			static const threading::longevity life_time = object::provider::life_time - 1;
			static const char name[];
		};
		
		const char envmgr::name[] = "environment";
	}
	
	
	bool environment::get( string &value, const string &name )
	{
		return envmgr::instance().query( value, name );
	}
	
	
	void environment::set( const string &name, const string &value)
	{
		envmgr::instance().store( name, value );
	}
	
	
	static void _parse_env( const char *str, const size_t len, void (*proc)( const string &, const string &, void *), void *args )
	{
		assert(str!=NULL);
		assert(len>0);
		size_t idx    = 1;
		bool   has_eq = false;
		while( idx < len ) {
			if( str[idx] == '=' ) {
				has_eq = true;
				break;
			}
			++idx;
		}
		if( !has_eq )
			throw libc::exception( EINVAL, "invalid system environment string");
		
		string name( str, idx );
		++idx;
		string value( str+idx, len-idx );
		proc( name, value, args );
    }
	
	
	void environment:: get( void (*proc)( const string &, const string &, void *), void *args )
	{
		
		YOCTO_LOCK(envmgr::access);
		assert( proc );
		
#if defined(YOCTO_BSD)
		char **env_reg = environ;
		while( *env_reg ) {
			const char *str = *env_reg;
			_parse_env( str, strlen(str), proc, args);
			++env_reg;
		}
#endif
		
		
#if defined(YOCTO_WIN)
		
		LPTCH es = 0;
		{
			YOCTO_GIANT_LOCK();
			es = ::GetEnvironmentStrings();
			if( !es )
				throw win32::exception( ::GetLastError(), "::GetEnvironmentStrings()" );
		}
		
		try {
			
			const char *str = (const char *)es;
			while( *str ) {
				size_t len = std::strlen( str );
				_parse_env( str, len, proc, args );
				str += len;
				++str;
			}
			
			YOCTO_GIANT_LOCK();
			::FreeEnvironmentStrings( es );
		}
		catch(...) {
			YOCTO_GIANT_LOCK();
			::FreeEnvironmentStrings( es );
			throw;
		}
		
#endif
	}
	
    
}

#include "yocto/string/conv.hpp"

namespace yocto
{
    template <>
    bool environment:: check<bool>( bool &value, const string &name)
    {
        string s;
        if( get(s, name) )
        {
            
            if(s=="false"||s=="FALSE"||s=="0")
            {
                value = false;
                return true;
            }
            
            if(s=="true"||s=="TRUE"||s=="1")
            {
                value = true;
            }
            
            throw imported::exception("environment::check<bool>","invalid value '%s'", s.c_str());
        }
        else
            return false;
    }
    
    template <>
    bool environment:: check<int>( int &value, const string &name)
    {
        string s;
        if( get(s,name) )
        {
            value = strconv::to<int>(value,"environment::check<int>");
            return true;
        }
        else
            return true;
    }
    
}
