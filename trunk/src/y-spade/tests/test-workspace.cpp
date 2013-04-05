#include "yocto/utest/run.hpp"
#include "yocto/spade/workspace.hpp"
#include "yocto/spade/rmesh.hpp"
#include "yocto/spade/vtk/writer.hpp"
#include "yocto/spade/variables.hpp"

#include "./main.hpp"

YOCTO_UNIT_TEST_IMPL(work1D)
{
    
    fields_setup<layout1D> F(2);
    ghosts_setup           G;
    layout1D               L( RandCoord1D(), RandCoord1D() );
    
    Y_SPADE_FIELD(F, "A", array1D<float>  );
    Y_SPADE_FIELD(F, "B", array1D<double> );
    
    G.set_local(on_x, 1);
    workspace<layout1D,rmesh,double> w(L,F,G);
    
    
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/array2d.hpp"

YOCTO_UNIT_TEST_IMPL(work2D)
{
    
    fields_setup<layout2D> F(2);
    ghosts_setup           G;
    layout2D               L( RandCoord2D(), RandCoord2D() );
    
    
    Y_SPADE_FIELD(F, "A", array2D<float>  );
    Y_SPADE_FIELD(F, "B", array2D<double> );
    
    G.set_local(on_x, 1);
    G.set_async(ghost::at_lower_y, 1, 0);
    G.set_async(ghost::at_upper_y, 1, 0);
    
    workspace<layout2D,rmesh,double> w(L,F,G);
    
    
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/spade/array3d.hpp"

YOCTO_UNIT_TEST_IMPL(work3D)
{
    
    fields_setup<layout3D> F(2);
    ghosts_setup           G;
    const coord3D          Lo( 0,0,0);
    const coord3D          Hi( 10,20,30);
    layout3D               L( Lo, Hi);
    
    Y_SPADE_FIELD(F, "A", array3D<float>  );
    Y_SPADE_FIELD(F, "B", array3D<double> );
    
    G.set_local(on_x, 1);
    G.set_async(ghost::at_lower_y, 1, 0);
    G.set_async(ghost::at_upper_y, 1, 0);
    G.set_local(on_z,1);
    
    workspace<layout3D,rmesh,float> W(L,F,G);
    
    
       
    
}
YOCTO_UNIT_TEST_DONE()
