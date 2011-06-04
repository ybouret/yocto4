#ifndef YOCTO_PRINTF_CHECK_INCLUDED
#define YOCTO_PRINTF_CHECK_INCLUDED 1


#if defined(__GNUC__)
//! \warning for C++ methods, increase count by 1!
#	define YOCTO_PRINTF_CHECK(i,j) __attribute__ ((format (printf, i, j)))
#else
#	define YOCTO_PRINTF_CHECK(i,j)
#endif


#endif
