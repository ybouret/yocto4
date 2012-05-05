#include "yocto/swamp/rwops.hpp"
#include "yocto/ios/ocstream.hpp"

#define ZTYPE float
#include "rwops.cxx"

#undef  ZTYPE
#define ZTYPE double
#include "rwops.cxx"
