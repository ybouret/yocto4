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

#if defined(YOCTO_WIN)
#include <cstdio>
extern "C" BOOL WINAPI DllMain(
                               HINSTANCE hinstDLL,  // handle to DLL module
                               DWORD fdwReason,     // reason for calling function
                               LPVOID lpReserved )  // reserved
{
    std::cerr << "In DllMain" << std::endl;
	puts("\t## C Code ##");
    // Perform actions based on the reason for calling.
    switch( fdwReason )
    {
        case DLL_PROCESS_ATTACH:
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.
            std::cerr << "[[ Enter Module ]]" << std::endl;
            break;
            
        case DLL_THREAD_ATTACH:
            // Do thread-specific initialization.
            std::cerr << "[[ Enter Module ]]/Thread" << std::endl;
            break;
            
        case DLL_THREAD_DETACH:
            // Do thread-specific cleanup.
            std::cerr << "[[ Leave Module ]]/Thread" << std::endl;
            break;
            
        case DLL_PROCESS_DETACH:
            // Perform any necessary cleanup.
            std::cerr << "[[ Leave Module ]]" << std::endl;
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#endif

static int add( int a, int b ) throw() { return a+b; }
static int mul( int a, int b ) throw() { return a*b; }

YOCTO_EXPORT void YOCTO_API load_ops( ops *api )
{
    assert(api!=0);
    api->add = add;
    api->mul = mul;
}

