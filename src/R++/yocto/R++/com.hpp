#ifndef YOCTO_RXX_COM_INCLUDED
#define YOCTO_RXX_COM_INCLUDED 1

#include "yocto/math/matrix.hpp"
#include "yocto/exception.hpp"

#if defined(YOCTO_WIN)
#undef ERROR
#endif

#include <R.h>
#include <Rinternals.h>


namespace yocto
{

    //__________________________________________________________________________
    //
    // Exceptions catcher in a R function
    //__________________________________________________________________________
#define YOCTO_R_EXPORT extern "C"
#define YOCTO_R_STATIC static inline

    //__________________________________________________________________________
    //
    // Wrappers to R function declarations...
    //__________________________________________________________________________

#define YOCTO_R_FUNCTION(NAME,ARGS) YOCTO_R_EXPORT SEXP NAME ARGS throw()\
{ static const char __fn[] = #NAME; \
try

#define YOCTO_R_RETURN() \
catch(const yocto::exception &e) { \
Rprintf("\t***[%s] %s\n", __fn, e.what());\
Rprintf("\t***[%s] %s\n", __fn, e.when());\
return R_NilValue;\
}\
catch(...) {\
Rprintf("\t***[%s] Unhandled exception!");\
return R_NilValue; }\
}

}


#endif
