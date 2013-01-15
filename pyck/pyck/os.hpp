#ifndef PYCK_OS_INCLUDED
#define PYCK_OS_INCLUDED 1

#define _FILE_OFFSET_BITS 64

#include <cassert>
#include <cstddef>

#if defined(_WIN32)
#	define PYCK_PLATFORM "Windows"
#	define PYCK_WIN		1
#	define _WIN32_WINNT    0x0501
#endif

#if defined(__MACH__) && ( defined(__APPLE__) || defined(__APPLE_CC__) )
#	define PYCK_PLATFORM	"Darwin"
#	define PYCK_BSD 		1
#	define PYCK_APPLE      1
#endif

#if defined(__linux__)
#	define PYCK_PLATFORM	"Linux"
#	define PYCK_BSD 		 1
#	define PYCK_LINUX       1
#endif

#if defined(__FreeBSD__)
#	define PYCK_PLATFORM	"FreeBSD"
#	define PYCK_BSD 		 1
#	define PYCK_FREEBSD     1
#endif

#if defined(__OpenBSD__)
#	define PYCK_PLATFORM	"OpenBSD"
#	define PYCK_BSD 		 1
#	define PYCK_OPENBSD     1
#endif

#if defined(__sun__)
#	define PYCK_PLATFORM	"SunOS"
#	define PYCK_BSD 		1
#	define PYCK_SUNOS      1
#endif

#if defined(__GNUC__) || defined(__DMC__)

/*--------------------------------------------------------------------*/
/*   ENTER: GNU systems                                               */
/*--------------------------------------------------------------------*/
#	include	<stdint.h>
#	define	PYCK_HAS_STDINT 1
#	define	PYCK_U64(X) X##ULL
#	define	PYCK_I64(X) X##LL
/*--------------------------------------------------------------------*/
/*   LEAVE: GNU systems                                               */
/*--------------------------------------------------------------------*/

#else

/*--------------------------------------------------------------------*/
/*   ENTER: other systems                                             */
/*--------------------------------------------------------------------*/
#if 	defined(_MSC_VER)

/* Microsoft built-in types */
typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
#		define	PYCK_HAS_STDINT 1
#		define	PYCK_U64(X) X##ui64
#		define	PYCK_I64(X) X##i64
#	endif /* _MSC_VER */

#endif

#if !defined(PYCK_HAS_STDINT)
#error "PYCK_HAS_STDINT failure"
#endif

//! integer to type mapping.
template <int v>
struct int2type {
	enum
	{
		value = v //!< a different class for each v.
	};
};

const char *GetPlatformName() throw();
uint32_t    IntegerHash32( uint32_t ) throw();

#if defined(__GNUC__)
//! \warning for C++ methods, increase count by 1!
#	define PYCK_PRINTF_CHECK(i,j) __attribute__ ((format (printf, i, j)))
#else
#	define PYCK_PRINTF_CHECK(i,j)
#endif

#define PYCK_ROUND_BYTE( ROUND_LN2 ) ( (size_t)( 1 << (ROUND_LN2) ) )
#define PYCK_ROUND_MASK( ROUND_LN2 ) ( (size_t)(PYCK_ROUND_BYTE( ROUND_LN2 )-0x1) )
#define PYCK_ROUND_NOT2( ROUND_LN2 ) ( (size_t)(~PYCK_ROUND_MASK( ROUND_LN2 ) )   )
#define PYCK_ROUND(ROUND_LN2,N)      ( ( (size_t)(N) + PYCK_ROUND_MASK(ROUND_LN2) ) & PYCK_ROUND_NOT2( ROUND_LN2 ) )

//! 1 byte  | 8 bits
#define PYCK_ROUND1(N)    PYCK_ROUND(0,N)

//! 2 bytes  | 16 bits
#define PYCK_ROUND2(N)    PYCK_ROUND(1,N)

//! 4 bytes  | 32 bits
#define PYCK_ROUND4(N)    PYCK_ROUND(2,N)

//! 8 bytes  | 64 bits
#define PYCK_ROUND8(N)    PYCK_ROUND(3,N)

//! 16 bytes | 128 bits
#define PYCK_ROUND16(N)   PYCK_ROUND(4,N)

//! 32 bytes | 256 bits
#define PYCK_ROUND32(N)   PYCK_ROUND(5,N)

//! 64 bytes | 512 bits
#define PYCK_ROUND64(N)   PYCK_ROUND(6,N)

//! 128 bytes | 1024 bits
#define PYCK_ROUND128(N)  PYCK_ROUND(7,N)

//! 256 bytes | 2048 bits
#define PYCK_ROUND256(N)  PYCK_ROUND(8,N)

#define PYCK_MEMALIGN(N)  PYCK_ROUND16(N)

#define PYCK_U64_FOR_SIZE(N) ( PYCK_ROUND8(N) >> 3 )
#define PYCK_U64_FOR_ITEM(T) PYCK_U64_FOR_SIZE(sizeof(T))

#define PYCK_U32_FOR_SIZE(N) ( PYCK_ROUND4(N) >> 2 )
#define PYCK_U32_FOR_ITEM(T) PYCK_U32_FOR_SIZE(sizeof(T))



#endif
