#ifndef YOCTO_ROUND_INCLUDED
#define YOCTO_ROUND_INCLUDED 1

#include "yocto/code/ilog2.hpp"

#define YOCTO_ROUND_BYTE( ROUND_LN2 ) ( (size_t)( 1 << (ROUND_LN2) ) )
#define YOCTO_ROUND_MASK( ROUND_LN2 ) ( (size_t)(YOCTO_ROUND_BYTE( ROUND_LN2 )-0x1) )
#define YOCTO_ROUND_NOT2( ROUND_LN2 ) ( (size_t)(~YOCTO_ROUND_MASK( ROUND_LN2 ) )   )
#define YOCTO_ROUND(ROUND_LN2,N)      ( ( (size_t)(N) + YOCTO_ROUND_MASK(ROUND_LN2) ) & YOCTO_ROUND_NOT2( ROUND_LN2 ) )

//! 1 byte  | 8 bits
#define YOCTO_ROUND1(N)    YOCTO_ROUND(0,N)

//! 2 bytes  | 16 bits
#define YOCTO_ROUND2(N)    YOCTO_ROUND(1,N)

//! 4 bytes  | 32 bits
#define YOCTO_ROUND4(N)    YOCTO_ROUND(2,N)

//! 8 bytes  | 64 bits 
#define YOCTO_ROUND8(N)    YOCTO_ROUND(3,N)

//! 16 bytes | 128 bits
#define YOCTO_ROUND16(N)   YOCTO_ROUND(4,N)

//! 32 bytes | 256 bits 
#define YOCTO_ROUND32(N)   YOCTO_ROUND(5,N)

//! 64 bytes | 512 bits
#define YOCTO_ROUND64(N)   YOCTO_ROUND(6,N)

//! 128 bytes | 1024 bits
#define YOCTO_ROUND128(N)  YOCTO_ROUND(7,N)

//! 256 bytes | 2048 bits
#define YOCTO_ROUND256(N)  YOCTO_ROUND(8,N)

//! internal memory alignment
#define YOCTO_MEMALIGN(N)  YOCTO_ROUND16(N)

#define YOCTO_U64_FOR_SIZE(N) ( YOCTO_ROUND8(N) >> 3 )
#define YOCTO_U64_FOR_ITEM(T) YOCTO_U64_FOR_SIZE(sizeof(T))

#define YOCTO_U32_FOR_SIZE(N) ( YOCTO_ROUND4(N) >> 2 )
#define YOCTO_U32_FOR_ITEM(T) YOCTO_U32_FOR_SIZE(sizeof(T))

#define YOCTO_BYTES_FOR(BITS) ( (size_t)(YOCTO_ROUND(3,BITS)) >>3 )

//! align on a power of two BYTES
#define YOCTO_ALIGN_FOR_SIZE(BYTES,N) YOCTO_ROUND(ilog2<BYTES>::value,N)

//! align on a primitive type, which has a power of two size
#define YOCTO_ALIGN_FOR_ITEM(T,N)     YOCTO_ROUND(ilog2_of<T>::value,N)

namespace yocto
{
    namespace memory
    {
        inline size_t align( size_t n ) throw() { return YOCTO_MEMALIGN(n); }
    }
}

#endif
