#include "yocto/math/kernel/tao.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(tao)
{
    vector<double> v1(4,0.0);
    
    tao::ld(v1,0.0);
    
}
YOCTO_UNIT_TEST_DONE()
