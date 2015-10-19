#include "yocto/R++/com.hpp"

YOCTO_R_STATIC
const char *get_version() throw()
{
    return __DATE__;
}

YOCTO_R_FUNCTION(the_version,())
{
    Rprintf("%s\n", get_version());
    return R_NilValue;
}
YOCTO_R_RETURN()
