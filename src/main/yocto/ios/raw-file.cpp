#include "yocto/ios/raw-file.hpp"
#include "yocto/exceptions.hpp"

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(YOCTO_BSD)
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#endif

//#define YOCTO_RAW_FILE_DEBUG

#if defined(YOCTO_RAW_FILE_DEBUG)
#include <iostream>
#	define YRAWFILE(CODE) CODE
#else
#	define YRAWFILE(CODE)
#endif

namespace yocto
{
	namespace ios
	{
		
        
		raw_file:: raw_file( const string &filename, size_t mode ) :
		local_file( is_regular ),
		handle( _fd::invalid() ),
		access( mode & ( readable|writable) ),
		status(NULL)
		{
#include "raw-file-inc.cxx"
		}
        
		raw_file:: raw_file( const char *fn, size_t mode ) :
		local_file( is_regular ),
		handle( _fd::invalid() ),
		access( mode & ( readable|writable) ),
		status(NULL)
		{
            const string filename(fn);
#include "raw-file-inc.cxx"
		}
		
        
        raw_file:: raw_file( handle_t user_handle, size_t user_access ) throw() :
        local_file( is_pipe ),
        handle( user_handle ),
        access( user_access & (readable|writable) ),
        status(NULL)
        {
            assert( handle != _fd::invalid() );
            assert( (access&readable) || (access&writable) );
        }
        
		raw_file:: ~raw_file() throw()
		{
			if( type == is_regular )
			{
				const error_type err = _fd::close( handle );
				if( status ) *status = err;
			}
			handle          = _fd::invalid();
			(size_t&)access = 0;
		}
		
		
		raw_file:: raw_file( const cstdin_t & ) :
		local_file( is_stdin ),
		handle(  _fd::invalid() ),
		access( readable ),
		status( NULL )
		{
#if defined(YOCTO_BSD)
			handle = STDIN_FILENO;
#endif
			
#if defined(YOCTO_WIN)
			handle = ::GetStdHandle(STD_INPUT_HANDLE);
#endif
		}
		
		raw_file:: raw_file( const cstdout_t & ) :
		local_file( is_stdout ),
		handle( _fd::invalid() ),
		access( writable ),
		status( NULL )
		{
#if defined(YOCTO_BSD)
			handle = STDOUT_FILENO;
#endif
			
#if defined(YOCTO_WIN)
			handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
#endif
		}
		
		raw_file:: raw_file( const cstderr_t & ) :
		local_file( is_stderr ),
		handle( _fd::invalid() ),
		access( writable ),
		status( NULL )
		{
#if defined(YOCTO_BSD)
			handle = STDERR_FILENO;
#endif
			
#if defined(YOCTO_WIN)
			handle = ::GetStdHandle(STD_ERROR_HANDLE);
#endif
		}
		
	}
    
}
