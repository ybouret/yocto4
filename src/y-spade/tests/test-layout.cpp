#include "yocto/utest/run.hpp"
#include "./main.hpp"

YOCTO_UNIT_TEST_IMPL(layout)
{
    
    offsets_list offsets;
    for( size_t i=0; i < 4; ++i )
    {
        layout1D L1( RandCoord1D(), RandCoord1D());
        std::cerr << "L1=" << L1 << std::endl;
        
        layout2D L2( RandCoord2D(), RandCoord2D() );
        std::cerr << "L2=" << L2 << std::endl;
        
        layout3D L3( RandCoord3D(), RandCoord3D() );
        std::cerr << "L3=" << L3 << std::endl;
        
    }
}
YOCTO_UNIT_TEST_DONE()
