#include "yocto/utest/run.hpp"
#include "yocto/spade/dataspace.hpp"
#include "yocto/spade/array3d.hpp"

using namespace yocto;
using namespace spade;

namespace
{
    template <typename DATASPACE>
    static inline void display( const DATASPACE &D, const char *name )
    {
        assert(name);
        std::cerr << name << ".layout : " << D.as_layout() << std::endl;
        std::cerr << name << ".outline: " << D.outline    << std::endl;
        std::cerr << "#" << name << ".sides = " << D.sides.size() << std::endl;
        for( size_t i=1; i <= D.sides.size(); ++i )
        {
            std::cerr << "\t" << name << ".sides[" << i << "]=" << D.sides[i] << std::endl;
        }
        std::cerr << std::endl;
    }
    
    static void ctrl1D( )
    {
        std::cerr << std::endl << "\t\t1D" << std::endl << std::endl;
        layout1D               L( 1, 10 );
        fields_setup<layout1D> F(16);
        {
            ghosts_setup           G;
            G.set_async( ghost::at_lower_x, 2, 0);
            dataspace<layout1D>  D(L,F,G);
            display(D, "1DXLO2");
        }
        
        {
            ghosts_setup           G;
            G.set_async( ghost::at_lower_x, 1, 0);
            G.set_async( ghost::at_upper_x, 2, 0);
            dataspace<layout1D>  D(L,F,G);
            display(D, "1DXLO1XHI2");
        }
    }
    
    static void ctrl2D( )
    {
        std::cerr << std::endl << "\t\t2D" << std::endl << std::endl;
        layout2D               L( coord2D(1,1), coord2D(10,20) );
        fields_setup<layout2D> F(16);
        {
            ghosts_setup           G;
            G.set_async( ghost::at_lower_y, 1, 0);
            G.set_async( ghost::at_upper_y, 2, 0);
            dataspace<layout2D>  D(L,F,G);
            display(D, "2DYLO1HI2");
        }
    }
    
    static void ctrl3D( )
    {
        std::cerr << std::endl << "\t\t3D" << std::endl << std::endl;
        layout3D               L( coord3D(1,1,1), coord3D(10,20,30) );
        fields_setup<layout3D> F(16);
        {
            ghosts_setup           G;
            G.set_async( ghost::at_lower_z, 1, 0);
            G.set_async( ghost::at_upper_z, 2, 0);
            dataspace<layout3D>  D(L,F,G);
            display(D, "3DZLO1ZHI2");
        }
    }

    
    
}


YOCTO_UNIT_TEST_IMPL(ctrls)
{
    
    ctrl1D();
    ctrl2D();
    ctrl3D();
    
}
YOCTO_UNIT_TEST_DONE()
