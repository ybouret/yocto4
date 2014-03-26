#include "yocto/utest/run.hpp"
#include "yocto/fovea/array2d.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/fovea/array-db.hpp"

using namespace yocto;
using namespace fovea;

YOCTO_UNIT_TEST_IMPL(adb)
{
    array_db adb;
    typedef array1D<double> A1d;
    typedef array2D<float>  A2f;
    
    for(int iter=1;iter<=3;++iter)
    {
        const unit_t lox = -unit_t(1 + alea_lt(100));
        const unit_t hix =  unit_t(1 + alea_lt(100));
        const unit_t loy = -unit_t(1 + alea_lt(100));
        const unit_t hiy =  unit_t(1 + alea_lt(100));
        const string n1 = vformat("n1-%d",iter);
        adb.store( new A1d( n1, layout1D(lox,hix)) );
        const string n2 = vformat("n2-%d",iter);
        adb.store( new A2f( n2, layout2D(coord2D(lox,loy),coord2D(hix,hiy) )) );
    }
    
    A1d &a1 = adb["n1-2"].as<A1d>();
    std::cerr << "a1=" << a1 << std::endl;
    
    A2f &a2 = adb["n2-1"].as<A2f>();
    std::cerr << "a2=" << a2 << std::endl;

    
}
YOCTO_UNIT_TEST_DONE()
