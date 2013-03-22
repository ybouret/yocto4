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
    
    void ctrl1D( )
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
}


YOCTO_UNIT_TEST_IMPL(ctrls)
{
    
    ctrl1D();
    
    
    
}
YOCTO_UNIT_TEST_DONE()
