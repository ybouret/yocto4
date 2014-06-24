#include "yocto/ios/file-descriptor.hpp"

#include "yocto/exceptions.hpp"
#include "yocto/threading/mutex.hpp"

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


#if defined(YOCTO_BSD)
#include <unistd.h>
#include <cerrno>
#endif


namespace yocto
{
	
#if defined(YOCTO_WIN)
	_fd::type _fd::invalid() throw()
	{
		return NULL;
	}
	
	error_type _fd::close(type handle) throw()
	{
		YOCTO_GIANT_LOCK();
		if( ! ::CloseHandle( handle ) )
		{
			return ::GetLastError();
		}
		return 0;
	}
	
#endif
	
#if defined(YOCTO_BSD)
	_fd::type _fd::invalid() throw()
	{
		return -1;
	}
	
	error_type _fd::close(type handle) throw()
	{
		YOCTO_GIANT_LOCK();
		while( ::close( handle ) != 0 )
		{
			const int err = errno;
			switch( err )
			{
				case EINTR:
					break;
					
				default:
					return err;
					break;
			}
		}
		return 0;
		
	}
	
#endif
	
	
	
	size_t _fd:: get( type handle, void *data, size_t size )
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
		if( ans != 0 )
		{
			throw os_exception( ans, "_fd::get" );
		}
        return static_cast<size_t>(ptr - (uint8_t *)data);
		
	}
	
	
	void _fd:: put( type handle, const void *data, size_t size, size_t &done )
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
			throw os_exception( ans, "_fd::put" );
		}
	}
	
	
	
}
