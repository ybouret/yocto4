#include "yocto/swamp/triangle.hpp"

#define ZTYPE float
#include "triangle.cxx"

#undef  ZTYPE
#define ZTYPE double
#include "triangle.cxx"
