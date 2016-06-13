#include "yocto/fame/split/quad-exchange.hpp"
#include "yocto/fame/mesh/quad-mesh.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/fame/array3d.hpp"

using namespace yocto;
using namespace fame;

YOCTO_UNIT_TEST_IMPL(grid)
{
    
    int ng = 2;
    linear_handles handles;
    const bool periodic[3] = { true, true, true };
    
    std::cerr << "-- 1D" << std::endl << std::endl;
    {
        const layout1D L(1,30);
        for(int size=1;size<=4;++size)
        {
            std::cerr << std::endl;
            std::cerr << "size=" << size << " : full=" << L << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                QuadGrid<layout1D> grid(L,periodic,rank,size,ng);
                std::cerr << "\trank=" << rank << " is " << grid.ranks << "/" << grid.sizes << std::endl;
                std::cerr << grid << std::endl;
             
                handles.free();
                handles.append( new array1D<double>("A1",grid.outline) );
                
                grid.allocate_async_for(handles.chunk_size);
                
                quad_exchange::data(grid,handles);
                
            }
        }
    }
    
    
    std::cerr << "-- 2D" << std::endl << std::endl;

    {
        const layout2D L( coord2D(1,1), coord2D(30,40) );
        for(int size=1;size<=8;++size)
        {
            std::cerr << std::endl;
            std::cerr << "size=" << size << " : full=" << L << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                QuadGrid<layout2D> grid(L,periodic,rank,size,ng);
                std::cerr << "\trank=" << rank << " is " << grid.ranks << "/" << grid.sizes << std::endl;
                std::cerr << grid << std::endl;
                
                handles.free();
                handles.append( new array2D<double>("A2",grid.outline) );
                
                grid.allocate_async_for(handles.chunk_size);
                
                quad_exchange::data(grid,handles);

            }
        }
    }
    
    
    std::cerr << "-- 3D" << std::endl << std::endl;
    {
    
        const layout3D L( coord3D(1,1,1), coord3D(30,40,50) );
        for(int size=1;size<=8;++size)
        {
            std::cerr << std::endl;
            std::cerr << "size=" << size << " : full=" << L << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                QuadGrid<layout3D> grid(L,periodic,rank,size,ng);
                std::cerr << "\trank=" << rank << " is " << grid.ranks << "/" << grid.sizes << std::endl;
                std::cerr << grid << std::endl;
                handles.free();
                
                handles.append( new array3D<double>("A3",grid.outline) );
                
                grid.allocate_async_for(handles.chunk_size);
                
                quad_exchange::data(grid,handles);

            }
        }
    }


    
    
}
YOCTO_UNIT_TEST_DONE()

