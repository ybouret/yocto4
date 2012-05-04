#include "yocto/swamp/level-set.hpp"
#include "yocto/exception.cpp"

#define ZTYPE float
#include "level-set.cxx"

#undef ZTYPE
#define ZTYPE double
#include "level-set.cxx"
