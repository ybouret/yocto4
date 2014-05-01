
#if defined(_WIN32)
#	define _WIN32_WINNT    0x0501
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	define JPEG_ON_WINDOWS
#	if defined(_MSC_VER)
#		define NEED_FAR_POINTERS
#	endif
#endif


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define HAVE_PROTOTYPES
#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT
#define HAVE_STDLIB_H


#ifdef JPEG_CJPEG_DJPEG

#define BMP_SUPPORTED
#define PPM_SUPPORTED
#define TARGA_SUPPORTED

#endif

