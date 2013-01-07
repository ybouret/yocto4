#ifndef PYCK_ERROR_INCLUDED
#define PYCK_ERROR_INCLUDED 1

#include "./os.hpp"

void LibcError( int err, char *buffer, size_t buflen ) throw();
#if defined(PYCK_WIN)
void Win32Error( int32_t err, char *buffer, size_t buflen ) throw();
#endif

#endif
