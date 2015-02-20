#include "yocto/math/alg/spike.hpp"

namespace yocto
{
    namespace math
    {
        spike::index_node::  index_node(size_t i) throw() : value(i), next(0), prev(0) {}
        spike::index_node:: ~index_node() throw() {}

    }

}

