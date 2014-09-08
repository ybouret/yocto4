#include "yocto/utest/run.hpp"
#include "yocto/container/tab2d.hpp"
#include "yocto/string.hpp"

using namespace yocto;
YOCTO_UNIT_TEST_IMPL(tab)
{
    Tableau1D<int> ti(0,5,0);
    for(size_t i=ti.cmin;i<=ti.cmax;++i)
    {
        ti[i] = int(i);
    }
    
    Tableau2D<float> tf(1,4,0,6, 0.0f);
    
    Tableau2D<string> ts(1,2,3,4, "Hello, World!");

}
YOCTO_UNIT_TEST_DONE()
