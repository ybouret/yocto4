#include "./os.hpp"

#include <R.h>
#include <Rmath.h>
#include <Rinternals.h>

extern "C"
SEXP toto( SEXP args ) throw()
{
    
    return R_NilValue;
}