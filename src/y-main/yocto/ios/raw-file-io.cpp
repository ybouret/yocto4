#include "yocto/ios/raw-file.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/utils.hpp"

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

#include <cstdio>

namespace yocto
{
	namespace ios
	{
		
		void raw_file:: get( void *data, size_t size, size_t &done )
		{
			_fd::get(handle,data,size,done);			
		}
		
		
		void raw_file:: put( const void *data, size_t size, size_t &done )
		{
			_fd::put(handle,data,size,done);
		}
		
		
		offset_t raw_file:: seek( offset_t delta, whence_t whence )
		{
			YOCTO_GIANT_LOCK();
#if defined(YOCTO_WIN)
			LARGE_INTEGER mov;
			LARGE_INTEGER res;
			mov.QuadPart = delta;
			res.QuadPart = 0;
#endif
			
			switch( whence )
			{
				case from_set:
				{
#if defined(YOCTO_BSD)
					const offset_t res = lseek( handle, delta, SEEK_SET);
					if( res < 0 )
						throw libc::exception( errno, "lseek(SEEK_SET)");
					return res;
#endif
					
#if defined(YOCTO_WIN)
					if( ! ::SetFilePointerEx( handle, mov, &res, FILE_BEGIN) )
						throw win32::exception( ::GetLastError(), "SetFilePointerEx(FILE_BEGIN)" );
					return res.QuadPart;
#endif
					
				}
					
				case from_cur:
				{
#if defined(YOCTO_BSD)
					const offset_t res = lseek( handle, delta, SEEK_CUR);
					if( res < 0 )
						throw libc::exception( errno, "lseek(SEEK_CUR)");
					return res;
#endif
					
#if defined(YOCTO_WIN)
					if( ! ::SetFilePointerEx( handle, mov, &res, FILE_CURRENT) )
						throw win32::exception( ::GetLastError(), "SetFilePointerEx(FILE_CURRENT)" );
					return res.QuadPart;
#endif
					
				}
					
				case from_end:
				{
#if defined(YOCTO_BSD)
					const offset_t res = lseek( handle, delta, SEEK_END);
					if( res < 0 )
						throw libc::exception( errno, "lseek(SEEK_END)");
					return res;
#endif
					
#if defined(YOCTO_WIN)
					if( ! ::SetFilePointerEx( handle, mov, &res, FILE_END) )
						throw win32::exception( ::GetLastError(), "SetFilePointerEx(FILE_END)" );
					return res.QuadPart;
#endif
				}
			}
			return 0;
		}
		
		
		offset_t raw_file:: tell()
		{
			return this->seek(0,from_cur); 
		}
		
		void raw_file:: rewind()
		{
			(void) this->seek(0,from_set);
		}
		
		void raw_file:: unwind()
		{
			(void) this->seek(0,from_end);
		}
		
		size_t raw_file:: length()
		{
			const offset_t now = this->tell();
			const offset_t ans = this->seek(0,from_end);
			if( now != this->seek( now, from_set ) )
				throw exception( "raw_file::length(bad reset)");
			return size_t(ans);
		}
		
		void raw_file:: get_all( void *data, size_t size )
		{
			assert( !( data==NULL && size>0 ) );
			uint8_t *p = static_cast<uint8_t*>(data);
			size_t   n = size;
			
			while( n > 0 )
			{
				size_t to_read = min_of<size_t>(n,BUFSIZ);
				size_t done    = 0;
				get(p,to_read,done);
				p += done;
				n -= done;
				if( done <= 0 )
					throw exception( "raw_file::get_all(unexpected EOF)");
			}
			
		}

		
	}
}
