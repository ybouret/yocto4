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

    tableau_of<int,matrix> tab(3,10,11);
    
}
YOCTO_UNIT_TEST_DONE()

