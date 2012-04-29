#define ZTYPE float
#include SOURCE

#undef ZTYPE
#define ZTYPE double
#include SOURCE

#undef  ZTYPE
#define ZTYPE math::complex<float>
#include SOURCE

#undef  ZTYPE
#define ZTYPE math::complex<double>
#include SOURCE

#undef ZTYPE
#define ZTYPE unit_t
#include SOURCE

#include "yocto/geom/v3d.hpp"

#undef ZTYPE
#define ZTYPE geom::v2d<float>
#include SOURCE

#undef ZTYPE
#define ZTYPE geom::v2d<double>
#include SOURCE

#undef ZTYPE
#define ZTYPE geom::v3d<float>
#include SOURCE

#undef ZTYPE
#define ZTYPE geom::v3d<double>
#include SOURCE