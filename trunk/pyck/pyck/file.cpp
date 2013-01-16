#define _CRT_SECURE_NO_WARNINGS

#include "./file.hpp"
#include "./error.hpp"
#include "./exception.hpp"

#include <cerrno>

cFile:: ~cFile() throw()
{
	assert(fp);
	fclose(fp);
	fp = 0;
}

Mutex cFile::Lock;

cFile:: cFile( const char *filename, OpenMode mode ) :
fp(0)
{
	assert(filename);
	PYCK_LOCK(Lock);
	const char *ms = 0;
	switch( mode )
	{
        case ReadOnly:
            ms = "rb";
            break;
            
        case Overwrite:
            ms = "wb";
            break;
            
        case Append:
            ms = "ab";
            break;
	}
	fp = fopen( filename, ms );
	if(!fp)
	{
		char errbuf[128];
		LibcFormatError(errno, errbuf, sizeof(errbuf) );
		throw Exception("fopen(%s,%s): %s", filename, ms, errbuf);
	}
}

#include <cstdarg>

void cFile:: operator()(const char *fmt,...)
{
	PYCK_LOCK(Lock);
	va_list ap;
	va_start(ap,fmt);
	const int ans = vfprintf(fp, fmt, ap);
	va_end(ap);
	if( ans < 0 )
	{
		char errbuf[128];
		LibcFormatError(errno, errbuf, sizeof(errbuf) );
		throw Exception("vfprintf: %s", errbuf);
	}
}

