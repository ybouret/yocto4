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

#define YOCTO_RAW_FILE_DEBUG

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
		handle( file_descriptor::invalid() ),
		access( mode & ( readable|writable) ),
		status(NULL)
		{
#if defined(YOCTO_BSD)
			int                 flags      = 0;
			static const mode_t unix_flags = (S_IWUSR | S_IRUSR) | (S_IRGRP) | (S_IROTH);
			
#endif
			
#if defined(YOCTO_WIN)
			DWORD dwDesiredAccess        = 0;
			DWORD dwShareMode            = FILE_SHARE_READ | FILE_SHARE_WRITE;
			DWORD dwCreationDisposition  = 0;
			DWORD dwFlagsAndAttributes   = FILE_ATTRIBUTE_NORMAL;
#endif
			
			/* Setup parameters */
			switch( mode )
			{
					
				case readable:	
					YRAWFILE(std::cerr << filename << ": readable" << std::endl);
#if defined(YOCTO_BSD)
					flags |= O_RDONLY;
#endif
					
#if defined(YOCTO_WIN)
					dwDesiredAccess        = GENERIC_READ;
					dwCreationDisposition  = OPEN_EXISTING;
					dwFlagsAndAttributes  |= FILE_FLAG_SEQUENTIAL_SCAN;
#endif
					
					break;
					
				case writable:	
					YRAWFILE(std::cerr << filename << ": writable" << std::endl);
#if defined(YOCTO_BSD)
					flags |= O_WRONLY | O_CREAT ;
#endif
					
#if defined(YOCTO_WIN)
					dwDesiredAccess       = GENERIC_WRITE;
					dwCreationDisposition = OPEN_ALWAYS;
					/* dwFlagsAndAttributes |=0; */
#endif
					
					break;
					
				case  readable | writable:		
					YRAWFILE(std::cerr << filename << ": readable|writable" << std::endl);
#if defined(YOCTO_BSD)
					flags |= O_RDWR | O_CREAT;
#endif
					
#if defined(YOCTO_WIN)
					dwDesiredAccess       = GENERIC_READ | GENERIC_WRITE;
					dwCreationDisposition = OPEN_ALWAYS;
					dwFlagsAndAttributes  = FILE_FLAG_RANDOM_ACCESS;
#endif
					
					break;
					
				case  writable | truncate:
					YRAWFILE(std::cerr << filename << ": writable|truncate" << std::endl);
#if defined(YOCTO_BSD)
					flags |= O_WRONLY | O_CREAT | O_TRUNC;
#endif
					
#if defined(YOCTO_WIN)
					dwDesiredAccess       = GENERIC_WRITE;
					dwCreationDisposition = CREATE_ALWAYS;
					/*dwFlagsAndAttributes |=0;*/
#endif
					
					break;
					
					
				case readable | writable | truncate:
					YRAWFILE(std::cerr << filename << ": readable|writable|truncate" << std::endl);
#if defined(YOCTO_BSD)
					flags |= O_RDWR | O_CREAT | O_TRUNC;
#endif
					
#if defined(YOCTO_WIN)
					dwDesiredAccess       = GENERIC_READ | GENERIC_WRITE;
					dwCreationDisposition = OPEN_ALWAYS  | TRUNCATE_EXISTING;
					dwFlagsAndAttributes  = FILE_FLAG_RANDOM_ACCESS;
#endif
					
					break;
					
				default:
#if defined(YOCTO_BSD)
					throw libc::exception( EINVAL, "ios::raw_file('%s',%u)", filename.c_str(),unsigned(mode));
#endif
					
#if defined(YOCTO_WIN)
					throw win32::exception( ERROR_INVALID_PARAMETER, "ios::raw_file('%s',%u)", filename.c_str(),unsigned(mode) );
#endif
					
					
			}
			
			/* system calls */
#if defined(YOCTO_BSD)
			YOCTO_GIANT_LOCK();
			while( (handle = open( filename.c_str(), flags, unix_flags) ) == file_descriptor::invalid() )
			{
				const int err = errno;
				switch( err )
				{
					case EINTR:
						break;
						
					default:
						throw libc::exception( err, "open('%s')", filename.c_str() );
				}
			}
#endif
			
#if defined(YOCTO_WIN)
			YOCTO_GIANT_LOCK();
			handle = CreateFile( TEXT(filename.c_str()),
								dwDesiredAccess,
								dwShareMode,
								NULL,
								dwCreationDisposition,
								dwFlagsAndAttributes,
								NULL);
			if(  file_descriptor::invalid() == handle )
			{
				throw win32:: exception( ::GetLastError(), "::CreateFile('%s')", filename.c_str() );
			}
#endif
			
			
		}
		
		
		raw_file:: ~raw_file() throw()
		{
			if( type == is_regular )
			{
				const error_type err = file_descriptor::close( handle );
				if( status ) *status = err;
			}
			handle          = file_descriptor::invalid();
			(size_t&)access = 0;
		}
		
		
		raw_file:: raw_file( const cstdin_t & ) :
		local_file( is_stdin ),
		handle(  file_descriptor::invalid() ),
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
		handle( file_descriptor::invalid() ),
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
		handle( file_descriptor::invalid() ),
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
