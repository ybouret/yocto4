#include "yocto/math/alg/spike2d.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include <cmath>

using namespace yocto;
using namespace math;

static inline double get_id(double x) { return x; }

YOCTO_UNIT_TEST_IMPL(spike2d)
{

    typedef spike2D<double,size_t> spike_t;
    typedef spike_t::pointer       spike_p;

    const size_t    n=30;
    const size_t    m=40;
    matrix<double>  M(n,m);
    vector<spike_p> spikes;

    for(size_t i=1;i<=n;++i)
    {
        const double x = (i*6.3)/n;
        for(size_t j=1;j<=m;++j)
        {
            const double y = (j*6.3)/n;
            M[i][j] = sin(x) * sin(y);
        }
    }


    spike_t::detect(spikes, M, 1, n, 1, m, get_id);

}
YOCTO_UNIT_TEST_DONE()
