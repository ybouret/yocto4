#include "yocto/utest/run.hpp"
#include "./main.hpp"
#include "yocto/spade/dataspace.hpp"
#include "yocto/spade/array3d.hpp"

YOCTO_UNIT_TEST_IMPL(ghosts)
{
    
    fields_setup<layout1D> F1(2);
    layout1D               L1( RandCoord1D(), RandCoord1D());
    ghosts_setup           G1a;
    
    Y_SPADE_FIELD(F1,"A1", array1D<float> );
    Y_SPADE_FIELD(F1,"B1", array1D<char>  );

    G1a.set_local_x(2);
    dataspace<layout1D> d1a( L1, F1, G1a);
    std::cerr << "d1a.layout : " << d1a.as_layout() << std::endl;
    std::cerr << "d1a.outline: " << d1a.outline     << std::endl;
    
    ghosts_setup  G1b;
    G1b.set_async( ghost::at_lower_x, 2, 0);
    dataspace<layout1D> d1b( L1, F1, G1b);
    std::cerr << "d1b.layout : " << d1b.as_layout() << std::endl;
    std::cerr << "d1b.outline: " << d1b.outline     << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()
