#include "yocto/visit/visit.hpp"


namespace yocto
{
    
    VisIt:: exception:: exception(const char *fmt,...) throw() :
				yocto::exception(),
				what_()
    {
        memset( what_, 0, sizeof(what_) );
        const char *msg = VisItGetLastError();
        if(!msg) msg = "VisIt: unknowm last error";
        strncpy(what_, msg, sizeof(what_)-1);
        va_list ap;
        va_start(ap,fmt);
        format(fmt,&ap);
    }
    
    VisIt:: exception:: ~exception() throw() {}
    
    VisIt:: exception:: exception(const exception &other ) throw() :
    yocto::exception(other),
    what_()
    {
        memcpy(what_,other.what_,sizeof(what_));
    }
    
}