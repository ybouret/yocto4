#include "yocto/swamp/array2d.hpp"
#include  <iostream>

#define ZTYPE float
#include "array2d.cxx"

#undef ZTYPE
#define ZTYPE double
#include "array2d.cxx"

