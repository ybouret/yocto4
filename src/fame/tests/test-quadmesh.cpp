#include "yocto/fame/mesh/quad-mesh.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace fame;

YOCTO_UNIT_TEST_IMPL(grid)
{
    
    int ng = 2;
    
    const bool periodic[3] = { true, true, true };
    
    {
        const layout1D L(1,30);
        for(int size=1;size<=4;++size)
        {
            std::cerr << std::endl;
            std::cerr << "size=" << size << " : full=" << L << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                QuadGrid<layout1D> grid(L,rank,size,ng,periodic);
                std::cerr << "\trank=" << rank << " is " << grid.ranks << "/" << grid.sizes << std::endl;
                std::cerr << grid << std::endl;
            }
        }
    }
    
    
    {
        const layout2D L( coord2D(1,1), coord2D(30,40) );
        for(int size=1;size<=8;++size)
        {
            std::cerr << std::endl;
            std::cerr << "size=" << size << " : full=" << L << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                QuadGrid<layout2D> grid(L,rank,size,ng,periodic);
                std::cerr << "\trank=" << rank << " is " << grid.ranks << "/" << grid.sizes << std::endl;
                std::cerr << grid << std::endl;
            }
        }
    }
    
    
    
    {
    
        const layout3D L( coord3D(1,1,1), coord3D(30,40,50) );
        for(int size=1;size<=9;++size)
        {
            std::cerr << std::endl;
            std::cerr << "size=" << size << " : full=" << L << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                QuadGrid<layout3D> grid(L,rank,size,ng,periodic);
                std::cerr << "\trank=" << rank << " is " << grid.ranks << "/" << grid.sizes << std::endl;
                std::cerr << grid << std::endl;
            }
        }
    }


    
    
}
YOCTO_UNIT_TEST_DONE()

