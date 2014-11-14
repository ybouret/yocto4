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
        fields_setup<layout1D> F1(16);
        layout1D               L1( RandCoord1D(), RandCoord1D());
        ghosts_setup           G1a;
        
        Y_SPADE_FIELD(F1,"A1", array1D<float> );
        Y_SPADE_FIELD(F1,"B1", array1D<char>  );
      
        
        G1a.set_local(on_x,2);
        dataspace<layout1D> d1a( L1, F1, G1a);
        std::cerr << "d1a.layout : " << d1a.as_layout() << std::endl;
        std::cerr << "d1a.outline: " << d1a.outline     << std::endl;
        std::cerr << "d1a.inboard: " << d1a.inboard     << std::endl;

        linear_handles handles;
        handles.append( d1a["A1"].handle() );
        handles.append( d1a["B1"].handle() );

        {
            array1D<float> &A = d1a["A1"].as< array1D<float> >();
            d1a.get_local(1).transfer( handles );
            d1a.get_local(1).transfer( d1a["A1"].handle() );
            
            for(unit_t x = A.lower; x <= A.upper; ++x )
                A[x] = float(x);
            
            std::cerr << "\t\tlocal initial: " << std::endl;
            display_array(A);
            d1a.get_local(1).transfer( handles );
            std::cerr << "\t\tlocal final  : " << std::endl;
            display_array(A);
        }
        
        ghosts_setup  G1b;
        G1b.set_async( ghost::at_lower_x, 2, 0);
        dataspace<layout1D> d1b( L1, F1, G1b);
        std::cerr << "d1b.layout : " << d1b.as_layout() << std::endl;
        std::cerr << "d1b.outline: " << d1b.outline     << std::endl;
        std::cerr << "d1b.inboard: " << d1b.inboard     << std::endl;
        handles.free();
        handles.append( d1b["A1"].handle() );
        handles.append( d1b["B1"].handle() );
        {
            array1D<float> &A = d1b["A1"].as< array1D<float> >();
            for(unit_t x = A.lower; x <= A.upper; ++x )
                A[x] = float(x);
            std::cerr << "\t\tasync initial: " << std::endl;
            display_array(A);
            
            //! store data into ghost inner
            d1b.get_async(1).inner_store( handles );
            std::cerr << "\t#IOBYTES=" <<  d1b.get_async(1).content << std::endl;
            
            //! artificial copy
            memcpy(d1b.get_async(1).obuffer,d1b.get_async(1).ibuffer, d1b.get_async(1).content);
            
            //! query data from the ghost outer
            d1b.get_async(1).outer_query(handles );
            
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
        fields_setup<layout2D> F2(16);
        layout2D               L2( RandCoord2D(), RandCoord2D());
        ghosts_setup           G2a;
        
        Y_SPADE_FIELD(F2,"A2", array2D<float> );
        Y_SPADE_FIELD(F2,"B2", array2D<char>  );
        
        G2a.set_local(on_x,2);
        dataspace<layout2D> d2a( L2, F2, G2a);
        std::cerr << "d2a.layout : " << d2a.as_layout() << std::endl;
        std::cerr << "d2a.outline: " << d2a.outline     << std::endl;
        std::cerr << "d2a.inboard: " << d2a.inboard     << std::endl;

        linear_handles handles;
        handles.append( d2a["A2"].handle() );
        handles.append( d2a["B2"].handle() );
        
        {
            handles.free();
            const char *names[] = { "A2", "B2" };
            d2a.query(handles, names, 2);
        }
        
        {
            array2D<float> &A = d2a["A2"].as< array2D<float> >();
            for( unit_t y=A.lower.y; y <= A.upper.y; ++y )
                for(unit_t x = A.lower.x; x <= A.upper.x; ++x )
                    A[y][x] = float(y+x);
            //double vmin = A.get_min();
            //double vmax = A.get_max();
            //A.ppm("a0.ppm", "a0", A, vprocf,NULL,vmin,vmax);
            d2a.get_local(1).transfer( handles );
            //A.ppm("a1.ppm","a",A,vprocf,NULL,vmin,vmax);
        }
        
        ghosts_setup           G2b;
        G2b.set_async( ghost::at_upper_y, 2, 0);
        G2b.set_async( ghost::at_lower_y, 2, 0);
        dataspace<layout2D> d2b( L2, F2, G2b);
        std::cerr << "d2b.layout : " << d2b.as_layout() << std::endl;
        std::cerr << "d2b.outline: " << d2b.outline     << std::endl;
        std::cerr << "d2b.inboard: " << d2b.inboard     << std::endl;
        handles.free();
        handles.append( d2b["A2"].handle() );
        handles.append( d2b["B2"].handle() );
        {
            array2D<float> &A = d2b["A2"].as< array2D<float> >();
            for( unit_t y=A.lower.y; y <= A.upper.y; ++y )
                for(unit_t x = A.lower.x; x <= A.upper.x; ++x )
                    A[y][x] = float(y+x);
           // double vmin = A.get_min();
           // double vmax = A.get_max();
            //A.ppm("b0.ppm", "b0", A, vprocf,NULL,vmin,vmax);
            d2b.get_async(1).inner_store( handles );
            const size_t num_io1 = d2b.get_async(1).content;
            d2b.get_async(2).inner_store( handles );
            const size_t num_io2 = d2b.get_async(2).content;
            std::cerr << "\tIOBYTES=" << num_io1 << "," << num_io2 << std::endl;
            if( num_io1 != num_io2 )
                throw exception("invalid 2D async");
            memcpy( d2b.get_async(2).obuffer, d2b.get_async(1).ibuffer, num_io1);
            memcpy( d2b.get_async(1).obuffer, d2b.get_async(2).obuffer, num_io2);
            d2b.get_async(1).outer_query( handles );
            d2b.get_async(2).outer_query( handles );
            
            //A.ppm("b1.ppm", "b1", A, vprocf,NULL,vmin,vmax);
        }
        
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
