#include "yocto/utest/run.hpp"
#include "./main.hpp"
#include "yocto/spade/dataspace.hpp"
#include "yocto/spade/array3d.hpp"
#include <cstring>

template <typename T>
static inline void display_array( const array1D<T> &A )
{
    for(unit_t x=A.lower;x<=A.upper;++x)
    {
        std::cerr << A[x] << " ";
    }
    std::cerr << std::endl;
}

static double vprocf( const float &x )
{
 
    return x;
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
        
        
        {
            array1D<float> &A = d1a["A1"].as< array1D<float> >();
            d1a.get_local(1).transfer( d1a.handles );
            d1a.get_local(1).transfer( d1a["A1"].handle() );
            
            for(unit_t x = A.lower; x <= A.upper; ++x )
                A[x] = x;
            
            std::cerr << "\t\tlocal initial: " << std::endl;
            display_array(A);
            d1a.get_local(1).transfer( d1a.handles );
            std::cerr << "\t\tlocal final  : " << std::endl;
            display_array(A);
        }
        
        ghosts_setup  G1b;
        G1b.set_async( ghost::at_lower_x, 2, 0);
        dataspace<layout1D> d1b( L1, F1, G1b);
        std::cerr << "d1b.layout : " << d1b.as_layout() << std::endl;
        std::cerr << "d1b.outline: " << d1b.outline     << std::endl;
        
        {
            array1D<float> &A = d1b["A1"].as< array1D<float> >();
            for(unit_t x = A.lower; x <= A.upper; ++x )
                A[x] = x;
            std::cerr << "\t\tasync initial: " << std::endl;
            display_array(A);
            
            //! store data into ghost inner
            const size_t num_io = d1b.get_async(1).inner_store( d1b.handles );
            std::cerr << "\t#IOBYTES=" << num_io << std::endl;
            
            //! artificial copy
            memcpy(d1b.get_async(1).obuffer,d1b.get_async(1).ibuffer,num_io);
            
            //! query data from the ghost outer
            d1b.get_async(1).outer_query( d1b.handles );
            
            std::cerr << "\t\tasync final  : " << std::endl;
            display_array(A);
        }
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
        {
            array2D<float> &A = d2a["A2"].as< array2D<float> >();
            for( unit_t y=A.lower.y; y <= A.upper.y; ++y )
                for(unit_t x = A.lower.x; x <= A.upper.x; ++x )
                    A[y][x] = y+x;
            double vmin = A.get_min();
            double vmax = A.get_max();
            A.ppm("a0.ppm", "a0", A, vprocf,NULL,vmin,vmax);
            d2a.get_local(1).transfer( d2a.handles );
            A.ppm("a1.ppm","a",A,vprocf,NULL,vmin,vmax);
        }
        
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
