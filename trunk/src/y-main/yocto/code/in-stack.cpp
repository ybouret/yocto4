#include "in-stack.hpp"

namespace yocto
{
    
    //------------------------------------------------------------------------
    // thread local storage
#ifdef _MSC_VER
#define THREADLOCAL __declspec(thread)
#else
#define THREADLOCAL
#endif    
    //------------------------------------------------------------------------
    
    /** Thread-specific stack bottom pointer. */
    THREADLOCAL static const char* s_stackBottom = 0;
    
    static ptrdiff_t getFrameSize( char *x ) throw()
    {
        char   y = 0;
        return x - &y;
    }
    
    void __stack::init() throw()
    {
        char x = 0;
        s_stackBottom = &x + getFrameSize(&x) * 2;
    }
    
    bool __stack::owns(const void *addr ) throw()
    {
        const char* p           = (const char*)addr;
        char        x           = 0;
        const char* stackTop    = &x;
        
        if ( stackTop > s_stackBottom )
            return p > s_stackBottom && p < stackTop; // stack grows up
        else
            return p > stackTop && p < s_stackBottom; // stack grows down
    }
    
    size_t __stack::used() throw()
    {
        char        x           = 0;
        const char* stackTop    = &x;
        
        if ( stackTop > s_stackBottom )
            return stackTop - s_stackBottom; // stack grows up
        else
            return s_stackBottom - stackTop; // stack grows down
    }
    
}