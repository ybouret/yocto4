#ifndef YOCTO_RTLD_EXPORT_INCLUDED
#define YOCTO_RTLD_EXPORT_INCLUDED 1

#include "yocto/os.hpp"

#if defined(YOCTO_WIN)
#define YOCTO_EXPORT extern "C" __declspec(dllexport)
#endif

#if defined(YOCTO_BSD)
#define YOCTO_EXPORT extern "C"
#endif

#endif
