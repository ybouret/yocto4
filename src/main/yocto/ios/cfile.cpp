#define _CRT_SECURE_NO_WARNINGS 1
#include "yocto/ios/cfile.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/memory/buffer.hpp"

#include <cerrno>

#if defined(YOCTO_WIN)
#include <io.h>
#include <fcntl.h>
#endif

namespace yocto
{
	
	namespace ios
	{
		
        FILE * cfile:: __get() throw()
        {
            assert(handle);
            return handle;
        }

		
		
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

        static inline void __set_binary_IO(FILE *handle)
        {
#if defined(YOCTO_WIN)
#if defined(_MSC_VER)
            _setmode( _fileno(handle) , O_BINARY);
#else
            setmode( fileno(handle) , O_BINARY);
#endif
#endif
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
			
            __set_binary_IO(handle);
        }
		
		cfile:: cfile( const cstdin_t  &) : local_file( is_stdin ), handle( stdin ), last_close(NULL)
		{
			if( !handle ) throw exception("no STDIN!");
            __set_binary_IO(handle);
		}

		cfile:: cfile( const cstdout_t  &) : local_file( is_stdout ), handle( stdout ), last_close(NULL)
		{
			if( !handle ) throw exception("no STDOUT!");
            __set_binary_IO(handle);
		}

		cfile:: cfile( const cstderr_t  &) : local_file( is_stderr ), handle( stderr ), last_close(NULL)
		{
			if( !handle ) throw exception("no STDERR!");
            __set_binary_IO(handle);
		}

		void cfile:: bufferize( memory::rw_buffer &buf )
		{
			YOCTO_GIANT_LOCK();
			if( setvbuf( handle, static_cast<char*>(buf.rw()), _IOFBF, buf.length() ) != 0 )
			{
				throw libc::exception( errno, "setvbuf(@%p+%u)", buf.rw(), unsigned(buf.length()) );
			}	
		}
        
        ptrdiff_t cfile:: tell() const throw()
        {
            assert(handle);
            return ftell(handle);
        }
		
	}
	
}
