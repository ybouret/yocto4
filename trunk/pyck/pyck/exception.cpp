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