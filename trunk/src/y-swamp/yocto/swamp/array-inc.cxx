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
