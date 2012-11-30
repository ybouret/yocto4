#ifndef YOCTO_RTLD_EXPORT_INCLUDED
#define YOCTO_RTLD_EXPORT_INCLUDED 1

#include "yocto/os.hpp"

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define YOCTO_API    WINAPI
#define YOCTO_EXPORT extern "C" __declspec(dllexport)
#endif

#if defined(YOCTO_BSD)
#define YOCTO_API
#define YOCTO_EXPORT extern "C"
#endif

#endif
