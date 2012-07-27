#include "yocto/math/types.hpp"

#define real_t float
#include "types.cxx"

#undef real_t
#define real_t double
#include "types.cxx"

