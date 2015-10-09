#include "yocto/math/types.hpp"
#include "yocto/xnumeric.hpp"

namespace yocto
{
    template <>
    float xnumeric<float>::minimum() { return math::numeric<float>::minimum; }

    template <>
    double xnumeric<double>::minimum() { return math::numeric<double>::minimum; }

}


