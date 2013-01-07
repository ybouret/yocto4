#include "./exception.hpp"
#include <cstring>
#include <cstdarg>
#include <cstdio>

#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif

Exception:: Exception(const char *fmt,...) throw() :
what_()
{
    memset(what_,0,Length);
    va_list args;
    va_start(args,fmt);
    vsnprintf(what_,Length-1,fmt,args);
    va_end(args);
}

Exception:: Exception( const Exception &excp ) throw() :
what_()
{
    memcpy( what_, excp.what_, Length );
}

Exception:: ~Exception() throw()
{
    
}

const char * Exception:: what() const throw() { return what_; }

#include "./error.hpp"

Exception:: Exception(const char *when, int64_t err ) throw()
{
    assert(when!=0);
    memset(what_,0,Length);
    strncpy(what_, when, Length);
    size_t       len = strlen(what_);
    const size_t top = Length-1;
    if(len<top) what_[len++] = ':';
    if(len<top) what_[len++] = ' ';
    char buffer[256];
    SystemFormatError(err, buffer, sizeof(buffer));
    const size_t msglen = strlen(buffer);
    for( size_t i=0; i<msglen; ++i)
    {
        if(len>=Length) break;
        what_[len++] = buffer[i];
    }
}
