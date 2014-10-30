#include "yocto/fame/split/quad1d.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/fame/split/quad2d.hpp"

using namespace yocto;
using namespace fame;

#include <cstdlib>

YOCTO_UNIT_TEST_IMPL(split)
{
    quad_links                 xlinks;
    quad1d::local_ghosts::list lg;
    {
        const layout1D l1 = layout1D(-5,5);
        
        for(int size=1;size<=8;++size)
        {
            std::cerr << std::endl;
            std::cerr << "size=" << size << ", full=" << l1 << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                const layout1D s = quad1d::split(l1, rank, size, true,xlinks);
                std::cerr << "\trank   =" << rank << ",\t sub=" << s << ",\t links=" << xlinks << std::endl;
                const layout1D s_out = quad1d::outline_of(s, rank, xlinks, 1, lg);
                std::cerr << "\toutline=" << s_out << std::endl;
            }
        }
    }
    
    return 0;
    
    quad_links ylinks;
    
    {
        const layout2D l2 = layout2D( coord2D(-5,-6), coord2D(5,6) );
        coord2D    sizes;
        coord2D    ranks;
        for(int size=1;size<=16;++size)
        {
            std::cerr << std::endl;
            std::cerr << "size=" << size << ", full=" << l2 << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                const layout2D s = quad2d::split(l2, rank, size, true,xlinks, true,ylinks,ranks,sizes);
                std::cerr << "\trank = " << rank << " : " << ranks << " / " << sizes << ", sub=" << s
                << " xlinks=" << xlinks << ", ylinks=" << ylinks
                << std::endl;
                
            }
            
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()