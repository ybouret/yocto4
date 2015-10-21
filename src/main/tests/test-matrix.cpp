#include "yocto/math/matrices.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/string/xnum.hpp"

using namespace yocto;
using namespace math;


YOCTO_UNIT_TEST_IMPL(matrix)
{
    matrix<float>  fm;
    matrix<double> fd(10,5);
    matrix<string> sm(8,7);
    matrix<int>    im(100,4);
    matrix<string> sr(50);

    matrix<double> f2(fd);
    fd = f2;

    matrices<int,matrix> tab(5,12,15);
    tab.ldz();
    for(size_t i=1;i<=tab.count;++i)
    {
        tab[i].neg();
    }
}
YOCTO_UNIT_TEST_DONE()

