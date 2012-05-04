#include "yocto/swamp/contour2d.hpp"
#include "yocto/code/utils.hpp"

#define ZTYPE float
#include "contour2d.cxx"

#undef ZTYPE
#define ZTYPE double
#include "contour2d.cxx"


