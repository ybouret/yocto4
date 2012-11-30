#include "module.hpp"
#include <iostream>


YOCTO_EXPORT int sum( int a, int b ) throw()
{
    return a+b;
}


#if defined(YOCTO_BSD)

__attribute__((constructor)) void OnInit()  
{
    std::cerr << "[[Entering Module]]" << std::endl;
}

__attribute__((destructor)) void OnQuit()
{
    std::cerr << "[[Leaving Module]]" << std::endl;
}
#endif
