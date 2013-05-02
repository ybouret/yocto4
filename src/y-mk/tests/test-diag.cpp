#include "yocto/utest/run.hpp"
#include "yocto/math/kernel/diag.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace math;



YOCTO_UNIT_TEST_IMPL(diag)
{

    matrix<double> a(4,4);
    
    for(size_t i=1; i<=a.rows; ++i )
    {
        for(size_t j=1; j<=a.cols;++j)
        {
            a[i][j] = (0.5-alea<double>());// + (i==j?a.rows:0);
        }
    }
    std::cerr << "a=" << a << std::endl;
    diag<double>::balance(a);
    std::cerr << "b=" << a << std::endl;
    diag<double>::Hessenberg(a);
    std::cerr << "c=" << a << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
