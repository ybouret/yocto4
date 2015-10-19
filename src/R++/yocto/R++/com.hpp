#ifndef YOCTO_RXX_COM_INCLUDED
#define YOCTO_RXX_COM_INCLUDED 1

#include "yocto/math/matrix.hpp"

#if defined(YOCTO_WIN)
#undef ERROR
#endif

#include <R.h>
#include <Rinternals.h>

#include "yocto/exception.hpp"

namespace yocto
{

#define YOCTO_R_PROLOG() try {

#define YOCTO_R_EPILOG() } \
catch(const yocto::exception &e) { \
Rprintf("\t*** %s\n",e.what());\
Rprintf("\t*** %s\n",e.when());\
return R_NilValue;\
}\
catch(...) { Rprintf("\t*** Unhandled exception !\n"); return R_NilValue; }


}


#endif
