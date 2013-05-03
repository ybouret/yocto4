#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/diag.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace math;



YOCTO_UNIT_TEST_IMPL(diag)
{

    matrix<double> a(4,4);
    const size_t   n = a.rows;
    for(size_t i=1; i<=n; ++i )
    {
        for(size_t j=1; j<=n;++j)
        {
            a[i][j] = (0.5-alea<double>());// + (i==j?a.rows:0);
        }
    }
    std::cerr << "a=" << a << std::endl;
    diag<double>::balance(a);
    std::cerr << "b=" << a << std::endl;
    diag<double>::Hessenberg(a);
    std::cerr << "c=" << a << std::endl;
    vector<double> wr(n,0);
    vector<double> wi(n,0);
    diag<double>::eigenvalues(a, wr,wi);
    
}
YOCTO_UNIT_TEST_DONE()
