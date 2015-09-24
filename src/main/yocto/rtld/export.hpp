#ifndef YOCTO_RTLD_EXPORT_INCLUDED
#define YOCTO_RTLD_EXPORT_INCLUDED 1

#include "yocto/os.hpp"

#if defined(__ICC)
#pragma warning ( disable : 1418 )
#endif

#if defined(YOCTO_WIN)
#define YOCTO_API    __cdecl
#define YOCTO_EXPORT extern "C" __declspec(dllexport)
#endif

#if defined(YOCTO_BSD)
#define YOCTO_API
#define YOCTO_EXPORT extern "C"
#endif

#define YOCTO_RTLD_LOADER yloader

//______________________________________________________________________________
//
// Specific Module Init/Quit
//______________________________________________________________________________

#if defined(YOCTO_BSD)

#define YOCTO_RTLD_SETUP(ON_INIT,ON_QUIT) \
__attribute__((constructor)) void OnInit() { ON_INIT(); } \
__attribute__((destructor))  void OnQuit() { ON_QUIT(); }

#endif

#if defined(YOCTO_WIN)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define YOCTO_RTLD_SETUP(ON_INIT,ON_QUIT) \
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID)\
{\
switch( fdwReason )\
{\
case DLL_PROCESS_ATTACH: ON_INIT(); break;\
case DLL_THREAD_ATTACH:  ON_INIT(); break;\
case DLL_THREAD_DETACH:  ON_QUIT(); break;\
case DLL_PROCESS_DETACH: ON_QUIT(); break;\
}\
return TRUE;\
}

#endif


#endif
