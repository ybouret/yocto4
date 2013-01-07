#define _CRT_SECURE_NO_WARNINGS 1
#include "./error.hpp"
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <iostream>

#if defined(PYCK_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#define snprintf _snprintf
#endif

static inline void clear_buffer( char *buffer ) throw()
{
    size_t len = strlen( buffer );
    while( len-- > 0 && isspace( buffer[len] ) ) buffer[len] = 0;
}

void LibcFormatError( int err, char *buffer, size_t buflen ) throw()
{
    assert( !(buffer==0 && buflen>0) );
    memset( buffer, 0, buflen );
    strncpy( buffer, strerror(err), buflen-1 );
    clear_buffer(buffer);
}

#define PYCK_ERRLEN 256

void LibcCriticalError(int err, const char *when ) throw()
{
    assert(when!=0);
    char buffer[PYCK_ERRLEN];
    LibcFormatError(err,buffer,sizeof(buffer));
    std::cerr << "*** LIBC Critical Error" << std::endl;
    std::cerr << "*** " << when   << std::endl;
    std::cerr << "*** " << buffer << std::endl;
    abort();
}

#if defined(PYCK_WIN)
void Win32FormatError( uint32_t err, char *buffer, size_t buflen ) throw()
{
    assert( !(buffer==0 && buflen>0) );
    memset( buffer, 0, buflen );
    ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    err,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    buffer,
                    buflen-1,
                    NULL );
    clear_buffer(buffer);
    if( strlen(buffer) == 0 )
    {
        snprintf(buffer,buflen-1,"[Error #%u]", (unsigned)err);
    }
}

void Win32CriticalError( uint32_t err, const char *when ) throw()
{
    assert(when!=0);
    char buffer[PYCK_ERRLEN];
    Win32FormatError(err,buffer,sizeof(buffer));
    std::cerr << "*** Win32 Critical Error" << std::endl;
    std::cerr << "*** " << when   << std::endl;
    std::cerr << "*** " << buffer << std::endl;
    abort();
}
#endif


void SystemFormatError( int64_t err, char *buffer, size_t buflen) throw()
{
#if defined(PYCK_WIN)
    Win32FormatError(uint32_t(err), buffer, buflen);
#endif
    
#if defined(PYCK_BSD)
    LibcFormatError(err, buffer, buflen);
#endif
    
}

void SystemCriticalError( int64_t err, const char *when ) throw()
{
#if defined(PYCK_WIN)
    Win32CriticalError( uint32_t(err),when);
#endif
    
#if defined(PYCK_BSD)
    LibcCriticalError(err, when);
#endif
}

