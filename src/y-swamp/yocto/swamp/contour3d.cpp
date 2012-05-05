#include "yocto/swamp/contour3d.hpp"

#define  ZTYPE float
#define  ZCORE marching_cubes_f
#include "contour3d.cxx"

#undef ZTYPE
#undef ZCORE
#define  ZTYPE double
#define  ZCORE marching_cubes_d
#include "contour3d.cxx"
