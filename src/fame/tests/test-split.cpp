#include "yocto/fame/split.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace fame;

#include <cstdlib>

YOCTO_UNIT_TEST_IMPL(split)
{
    const layout1D l1 = layout1D(-5,5);
    Links lx;
    for(int size=1;size<=6;++size)
    {
        const string dotfn = vformat("s1d%d.dot",size);
        {
            ios::ocstream fp( dotfn, false);
            fp("digraph G {\n");
            fp("rankdir=LR;\n");
            fp("ordering=out;\n");
            std::cerr << std::endl;
            std::cerr << "size="  << size << " : full= " << l1 << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                const layout1D l = Split(l1, rank, size,true,lx,NULL);
                std::cerr << "\trank="  << rank << " : " << l << ",\tlinks=" << lx << std::endl;
                fp("\t%d [pos=\"%g,0!\"];\n",rank, double(rank));
                for(size_t i=0;i<lx.count;++i)
                {
                    fp("\t\t%d -> %d;\n", rank, lx[i].rank);
                }
            }
            fp("}\n");
        }
        const string cmd = "dot -Tpng " + dotfn + " -o " + vformat("s1d%d.png",size);
        (void)system( cmd.c_str() );
        
    }
    
    //return 0;
    
    const layout2D l2 = layout2D( coord2D(-5,-6), coord2D(5,6) );
    Links ly;
    
    int ranks[4];
    for(int size=1; size<=16;++size)
    {
        const string dotfn = vformat("s2d%d.dot",size);
        
        {
            ios::ocstream fp( dotfn, false);
            fp("digraph G {\n");
            fp("rankdir=LR;\n");
            fp("ordering=out;\n");
            std::cerr << std::endl;
            std::cerr << "size="  << size << " : full= " << l2 << std::endl;
            for(int rank=0;rank<size;++rank)
            {
                const layout2D l = Split(l2, rank, size, true, lx, true, ly,ranks);
                std::cerr << "\trank=" << rank << " is (" << ranks[0] << "," << ranks[1] << ") : " << l << ",\txlinks=" <<lx << ",\tylinks=" << ly << std::endl;
                fp("\t%d;\n",rank);
                for(size_t i=0;i<lx.count;++i)
                {
                    fp("\t\t%d -> %d;\n", rank, lx[i].rank);
                }
                for(size_t i=0;i<ly.count;++i)
                {
                    fp("\t\t%d -> %d;\n", rank, ly[i].rank);
                }

            }
            fp("}\n");
        }
        const string cmd = "dot -Tpng " + dotfn + " -o " + vformat("s2d%d.png",size);
        (void)system( cmd.c_str() );
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()