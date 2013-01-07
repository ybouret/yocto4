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

#endif
