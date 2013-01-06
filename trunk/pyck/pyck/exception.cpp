#include "./exception.hpp"
#include <cstring>

Exception:: Exception() throw() :
what_()
{
    memset(what_,0,Length);
}

Exception:: Exception( const Exception &excp ) throw() :
what_()
{
    memcpy( what_, excp.what_, Length );
}

Exception:: ~Exception() throw()
{
    
}