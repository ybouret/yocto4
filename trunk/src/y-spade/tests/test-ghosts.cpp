#include "yocto/utest/run.hpp"
#include "./main.hpp"
#include "yocto/spade/dataspace.hpp"
#include "yocto/spade/array3d.hpp"

template <typename T>
static inline void display_array( const array1D<T> &A )
{
    for(unit_t x=A.lower;x<=A.upper;++x)
    {
        std::cerr << A[x] << " ";
    }
    std::cerr << std::endl;
}

YOCTO_UNIT_TEST_IMPL(ghosts)
{
    
    ////////////////////////////////////////////////////////////////////////////
    //
    // 1D
    //
    ////////////////////////////////////////////////////////////////////////////
    {
        fields_setup<layout1D> F1(2);
        layout1D               L1( RandCoord1D(), RandCoord1D());
        ghosts_setup           G1a;
        
        Y_SPADE_FIELD(F1,"A1", array1D<float> );
        Y_SPADE_FIELD(F1,"B1", array1D<char>  );
        
        G1a.set_local(on_x,2);
        dataspace<layout1D> d1a( L1, F1, G1a);
        std::cerr << "d1a.layout : " << d1a.as_layout() << std::endl;
        std::cerr << "d1a.outline: " << d1a.outline     << std::endl;
        
        
        array1D<float> &A = d1a["A1"].as< array1D<float> >();
        d1a.get_local(1).transfer( d1a.handles );
        d1a.get_local(1).transfer( d1a["A1"].handle() );
        
        for(unit_t x = A.lower; x <= A.upper; ++x )
            A[x] = x;
        
        std::cerr << "\t\tinitial: " << std::endl;
        display_array(A);
        d1a.get_local(1).transfer( d1a.handles );
        std::cerr << "\t\tfinal  : " << std::endl;
        display_array(A);
        
        
        ghosts_setup  G1b;
        G1b.set_async( ghost::at_lower_x, 2, 0);
        dataspace<layout1D> d1b( L1, F1, G1b);
        std::cerr << "d1b.layout : " << d1b.as_layout() << std::endl;
        std::cerr << "d1b.outline: " << d1b.outline     << std::endl;
    }
    
    std::cerr << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    //
    // 2D
    //
    ////////////////////////////////////////////////////////////////////////////
    {
        fields_setup<layout2D> F2(2);
        layout2D               L2( RandCoord2D(), RandCoord2D());
        ghosts_setup           G2a;
        
        Y_SPADE_FIELD(F2,"A2", array2D<float> );
        Y_SPADE_FIELD(F2,"B2", array2D<char>  );
        
        G2a.set_local(on_x,2);
        dataspace<layout2D> d2a( L2, F2, G2a);
        std::cerr << "d2a.layout : " << d2a.as_layout() << std::endl;
        std::cerr << "d2a.outline: " << d2a.outline     << std::endl;
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
