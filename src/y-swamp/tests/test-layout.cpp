
#include "yocto/utest/run.hpp"

#include "yocto/swamp/in1d.hpp"
#include "yocto/swamp/in2d.hpp"
#include "yocto/swamp/in3d.hpp"

#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace swamp;



static inline coord1D rand1()
{
    return coord1D(50 - 100 * alea<double>());
}

static inline coord2D rand2()
{
    return coord2D( rand1(), rand1() );
}

static inline coord3D rand3()
{
    return coord3D( rand1(), rand1(), rand1() );
}

YOCTO_UNIT_TEST_IMPL(layout)
{
    std::cerr << "1D" << std::endl;
    for( int i=0; i < 8; ++i )
    {
        const layout1D L( rand1(), rand1() );
        std::cerr << L << std::endl;
    }
    
    std::cerr << std::endl;
    std::cerr << "2D" << std::endl;
    for( int i=0; i < 8; ++i )
    {
        const layout2D L( rand2(), rand2() );
        std::cerr << L << std::endl;
    }
    
    std::cerr << std::endl;
    std::cerr << "3D" << std::endl;
    for( int i=0; i < 8; ++i )
    {
        const layout3D L( rand3(), rand3() );
        std::cerr << L << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
