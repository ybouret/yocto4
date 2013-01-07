#include "./error.hpp"
#include <cstring>

void LibcError( int err, char *buffer, size_t buflen ) throw()
{
    assert( !(buffer==0 && buflen>0) );
    strerror_r(err, buffer, buflen);
}