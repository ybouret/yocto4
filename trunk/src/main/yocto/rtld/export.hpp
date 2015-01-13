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


#endif
