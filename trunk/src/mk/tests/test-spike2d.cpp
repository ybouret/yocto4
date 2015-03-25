#include "yocto/math/alg/spike2d.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include <cmath>

using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(spike2d)
{


    const size_t n = 70;
    const size_t m = 90;

    matrix<double> M(n,m);
    for(size_t i=1;i<=n;++i)
    {
        const double x = (i*6.3)/n;
        for(size_t j=1;j<=m;++j)
        {
            const double y = (j*6.3)/n;
            M[i][j] = sin(x) * sin(y);
        }
    }

    spike2d::detect(M, size_t(1), n, size_t(1), m);

}
YOCTO_UNIT_TEST_DONE()
