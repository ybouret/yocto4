#ifndef YOCTO_FOURCC_INLUDED
#define YOCTO_FOURCC_INCLUDED 1

#include "yocto/os.hpp"

#define _YOCTO_DW(X)	( (uint32_t)(X) )
#define _YOCTO4CC(X,S)	( _YOCTO_DW(X) << ( (uint8_t)(S) ) )
#define YOCTO_FOURCC(A,B,C,D) ( _YOCTO4CC(A,24) | _YOCTO4CC(B,16) | _YOCTO4CC(C,8) | _YOCTO4CC(D,0) )


#endif
