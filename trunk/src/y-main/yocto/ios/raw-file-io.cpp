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

namespace yocto
{
	namespace ios
	{
		
		void raw_file:: get( void *data, size_t size, size_t &done )
		{
			assert( !(data==NULL&&size>0) );
			uint8_t   *ptr = (uint8_t *)data;
			size_t     len =            size;
			error_type ans = 0;
			
			while( len > 0 )
			{
#if defined(YOCTO_BSD)
				YOCTO_GIANT_LOCK();
				ssize_t res = 0;
				while( ( res = read( handle, ptr, len )  ) < 0 )
				{
					const int err = errno;
					if( EINTR == err )
						continue; /* interrupted primitive */
					else
					{
						ans = err; /* I/O error */
						goto DONE_GET;
					}
				}
				
				if( res > 0 )
				{
					len -= res;
					ptr += res;
				}
				else
					goto DONE_GET; /* no error, but couldn't read ! */
#endif
				
#if defined(YOCTO_WIN)
				YOCTO_GIANT_LOCK();
				DWORD bytesToRead  = (DWORD)( len & 0x0fffffff);
				DWORD bytesRead    = 0;
				BOOL  res = ::ReadFile(handle,
									   ptr,
									   bytesToRead,
									   &bytesRead,
									   NULL);
				ptr += bytesRead;
				len -= bytesRead;
				if( !res )
				{
					ans = ::GetLastError();
					if( ans == ERROR_BROKEN_PIPE )
					{
						ans = 0;
					}
					goto DONE_GET;
				}
				if( bytesRead <= 0 )
					goto DONE_GET;
#endif
				
				
			}
			
		DONE_GET:
			done = static_cast<size_t>(ptr - (uint8_t *)data);
			if( ans != 0 )
			{
				throw os_exception( ans, "raw_file::get" );
			}
			
		}
		
		
		void raw_file:: put( const void *data, size_t size, size_t &done )
		{
			assert( !(data==NULL&&size>0) );
			const uint8_t *ptr = (const uint8_t *)data;
			size_t         len =            size;
			error_type     ans = 0;
			while( len > 0 )
			{
				
#if defined(YOCTO_BSD)
				YOCTO_GIANT_LOCK();
				ssize_t res = 0;
				while( ( res = write( handle, ptr, len )  ) < 0 )
				{
					const int err = errno;
					if( EINTR == err )
						continue; /* interrupted primitive */
					else
					{
						ans = err; /* I/O error */
						goto DONE_PUT;
					}
				}
				
				if( res > 0 )
				{
					len -= res;
					ptr += res;
				}
				else
					goto DONE_PUT; /* no error, but couldn't write ! */
#endif
				
#if defined(YOCTO_WIN)
				YOCTO_GIANT_LOCK();
				DWORD bytesToWrite = (DWORD)( len & 0x0fffffff);
				DWORD bytesWritten = 0;
				BOOL  res = ::WriteFile(handle,
										ptr,
										bytesToWrite,
										&bytesWritten,
										NULL);
				ptr += bytesWritten;
				len -= bytesWritten;
				if( !res )
				{
					ans = ::GetLastError();
#if 0
					if( ans == ERROR_BROKEN_PIPE )
					{
						ans = 0;
					}
#endif
					goto DONE_PUT;
				}
				if( bytesWritten <= 0 )
					goto DONE_PUT;
#endif
				
			}
			
		DONE_PUT:
			done = static_cast<size_t>(ptr - (const uint8_t *) data);
			if( ans != 0 )
			{
				throw os_exception( ans, "raw_file::put" );
			}
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
			return ans;
		}
		
		
	}
}
