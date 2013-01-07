#ifndef PYCK_ERROR_INCLUDED
#define PYCK_ERROR_INCLUDED 1

#include "./os.hpp"

void LibcFormatError(int err, char *buffer, size_t buflen ) throw();
void LibcCriticalError(int err, const char *when ) throw();

#if defined(PYCK_WIN)
void Win32FormatError(uint32_t err, char *buffer, size_t buflen ) throw();
void Win32CriticalError(uint32_t err, const char *when) throw();
#endif

void SystemFormatError( int64_t err, char *buffer, size_t buflen) throw();
void SystemCriticalError( int64_t err, const char *when ) throw();


#endif
