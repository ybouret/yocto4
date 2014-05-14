#include "yocto/utest/run.hpp"
#include "yocto/container/tab2d.hpp"

using namespace yocto;
YOCTO_UNIT_TEST_IMPL(tab)
{
    Tableau1D<int> ti(0,5,0);
    for(size_t i=ti.imin;i<=ti.imax;++i)
    {
        ti[i] = int(i);
    }
    
    Tableau2D<float> tf(1,4,0,6, 0.0f);
    

}
YOCTO_UNIT_TEST_DONE()

