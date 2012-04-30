#include "yocto/swamp/region.hpp"
#include "yocto/code/swap.hpp"

#define UTYPE float
#include "region.cxx"

#undef  UTYPE
#define UTYPE double
#include "region.cxx"
