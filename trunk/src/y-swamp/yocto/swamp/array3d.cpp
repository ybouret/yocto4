#include "yocto/swamp/array3d.hpp"
#include  <iostream>

#define ZTYPE float
#include "array3d.cxx"

#undef ZTYPE
#define ZTYPE double
#include "array3d.cxx"

