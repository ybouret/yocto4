#include "yocto/R/R++.hpp"

using namespace yocto;

extern "C"
SEXP lyre_version() throw()
{
    Rprintf("1\n");
    return R_NilValue;
}

