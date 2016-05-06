#ifndef YOCTO_OS_INCLUDED
#define YOCTO_OS_INCLUDED 1

#define _FILE_OFFSET_BITS 64

#if defined(__cplusplus)
#include <cassert>
#include <cstddef>
#else
#include <assert.h>
#include <stddef.h>
#endif


#define YOCTO_SO_EXT "so"

#if defined(_WIN32)
#	define YOCTO_PLATFORM "Windows"
#	define YOCTO_WIN		1
#   if defined(_WIN32_WINNT)
#     undef  _WIN32_WINNT
#	  define _WIN32_WINNT    0x0501
#   endif
#   undef  YOCTO_SO_EXT
#   define YOCTO_SO_EXT "dll"
#endif

#if defined(__MACH__) && ( defined(__APPLE__) || defined(__APPLE_CC__) )
#	define YOCTO_PLATFORM	"Darwin"
#	define YOCTO_BSD 		1
#	define YOCTO_APPLE      1
#endif

#if defined(__linux__)
#	define YOCTO_PLATFORM	"Linux"
#	define YOCTO_BSD 		 1
#	define YOCTO_LINUX       1
#endif

#if defined(__FreeBSD__)
#	define YOCTO_PLATFORM	"FreeBSD"
#	define YOCTO_BSD 		 1
#	define YOCTO_FREEBSD     1
#endif

#if defined(__OpenBSD__)
#	define YOCTO_PLATFORM	"OpenBSD"
#	define YOCTO_BSD 		 1
#	define YOCTO_OPENBSD     1
#endif

#if defined(__sun__)
#	define YOCTO_PLATFORM	"SunOS"
#	define YOCTO_BSD 		1
#	define YOCTO_SUNOS      1
#endif

#if defined(__GNUC__) || defined(__DMC__)

/*--------------------------------------------------------------------*/
/*   ENTER: GNU systems                                               */
/*--------------------------------------------------------------------*/
#	include	<stdint.h>
#	define	YOCTO_HAS_STDINT 1
#	define	YOCTO_U64(X) X##ULL
#	define	YOCTO_I64(X) X##LL
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
#		define	YOCTO_HAS_STDINT 1
#		define	YOCTO_U64(X) X##ui64
#		define	YOCTO_I64(X) X##i64
#	endif /* _MSC_VER */

#endif

#if !defined(YOCTO_HAS_STDINT)
#error "YOCTO_HAS_STDINT failure"
#endif

#if defined(__cplusplus)

namespace yocto
{
	
	const char *platform_name() throw();
	uint32_t    hash32( const void *buffer, size_t buflen ) throw();
	
#define YOCTO_DISABLE_COPY(CLASS)   private: CLASS(const CLASS &)
#define YOCTO_DISABLE_ASSIGN(CLASS) private: CLASS &operator=( const CLASS & )
#define YOCTO_DISABLE_COPY_AND_ASSIGN(CLASS) \
YOCTO_DISABLE_COPY(CLASS);\
YOCTO_DISABLE_ASSIGN(CLASS)
	
	//! integer to type mapping.
	template <int v>
	struct int2type {
		enum
		{
		    value = v //!< a different class for each v.
		};
	};
	
	//! type to type mapping.
	template <class T>
	struct type2type
	{
		typedef T original_type; //!< a different light weight class for each T.
	};
	
	typedef type2type<size_t>   as_capacity_t; //!< for constructors
	extern const as_capacity_t  as_capacity;   //!< for constructors
	
	//! default type selector.
	template <const bool flag, typename T, typename U>
	struct select
	{
		typedef T result; //!< flag is true
	};
	
	//! specialized type selector.
	template <typename T, typename U>
	struct select<false,T,U>
	{
		typedef U result; //!< flag is false
	};
	
		
	template <typename T>
	inline void destruct( T *item ) throw()
	{
		assert(item);
		item->~T();
	}
	
	inline bool die( const char *) throw() { return false; }
	
    typedef ptrdiff_t unit_t;
}
#endif

#endif
