#include "yocto/swamp/array1d.hpp"
#include  <iostream>

#define ZTYPE float
#include "array1d.cxx"

#undef ZTYPE
#define ZTYPE double
#include "array1d.cxx"

