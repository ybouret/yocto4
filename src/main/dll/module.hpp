#ifndef DLL_MODULE_INCLUDED
#define DLL_MODULE_INCLUDED 1

#include "yocto/rtld/export.hpp"

//int sum(int a, int b) throw();

struct ops
{
    int (*add)(int,int);
    int (*mul)(int,int);
};

#endif
