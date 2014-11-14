#include "yocto/utest/run.hpp"
#include "yocto/spade/workspace.hpp"
#include "yocto/spade/rmesh.hpp"

#include "./main.hpp"
#include "yocto/spade/array2d.hpp"
#include "yocto/spade/variables.hpp"

YOCTO_UNIT_TEST_IMPL(io)
{
    fields_setup<layout2D> F(2);
    ghosts_setup           G;
    layout2D               L( RandCoord2D(), RandCoord2D() );
    
    
    Y_SPADE_FIELD(F, "A", array2D<double> );
    Y_SPADE_FIELD(F, "B", array2D<double> );
    
    G.set_local(on_x, 1);
    G.set_async(ghost::at_lower_y, 1, 0);
    G.set_async(ghost::at_upper_y, 1, 0);
    
    workspace<layout2D,rmesh,double> w(L,F,G);
    offsets_list inside;

    w.outline.load_offsets( w.as_layout(), inside);
    
    vector<double> v(2,0);
    linear_handles h;
    variables      var;
    var << "A" << "B";
    w.query(h, var);
    
    for( size_t i=inside.size();i>0;--i)
    {
        const size_t j = inside[i];
        w.load(v, h, j);
        v[1] = alea<double>();
        v[2] = alea<double>();
        w.save(h,v,j);
    }
    
}
YOCTO_UNIT_TEST_DONE()
