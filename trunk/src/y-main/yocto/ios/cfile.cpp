#define _CRT_SECURE_NO_WARNINGS 1
#include "yocto/ios/cfile.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/memory/buffer.hpp"

#include <cerrno>

namespace yocto
{
	
	namespace ios
	{
		
		
		
		
		cfile:: ~cfile() throw()
		{
			if( is_regular == type )
			{
				YOCTO_GIANT_LOCK();
				last_close = 0;
				if( fclose(handle) != 0 )
				{
					if( last_close )
						*last_close = errno;
				}
			}
		}
		
		cfile:: cfile( const char *filename, const char *mode ) :
		local_file( is_regular ),
		handle( NULL ),
		last_close( NULL )
		{
			assert(filename);
			assert(mode);
			YOCTO_GIANT_LOCK();
			handle = fopen( filename, mode );
			
			if( !handle )
			{
				throw libc::exception( errno, "fopen(%s,%s)", filename,mode );
			}
			
#if defined(YOCTO_WIN)
#if defined(_MSC_VER)
			_setmode( _fileno(handle) , O_BINARY);
#else
			setmode( fileno(handle) , O_BINARY);
#endif
#endif
		}
		
		cfile:: cfile( const cstdin_t  &) : local_file( is_stdin ), handle( stdin ), last_close(NULL)
		{
			if( !handle ) throw exception("no STDIN!");
		}
		
		cfile:: cfile( const cstdout_t  &) : local_file( is_stdout ), handle( stdout ), last_close(NULL)
		{
			if( !handle ) throw exception("no STDOUT!");
		}
		
		cfile:: cfile( const cstderr_t  &) : local_file( is_stderr ), handle( stderr ), last_close(NULL)
		{
			if( !handle ) throw exception("no STDERR!");
		}
		
		void cfile:: bufferize( memory::rw_buffer &buf )
		{
			YOCTO_GIANT_LOCK();
			if( setvbuf( handle, static_cast<char*>(buf.rw()), _IOFBF, buf.length() ) != 0 )
			{
				throw libc::exception( errno, "setvbuf(@%p+%u)", buf.rw(), unsigned(buf.length()) );
			}	
		}
		
	}
	
}
