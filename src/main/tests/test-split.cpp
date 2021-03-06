#include "yocto/utest/run.hpp"
#include "yocto/parallel/setup.hpp"
#include "yocto/parallel/basic.hpp"

#include "yocto/string/conv.hpp"
#include "yocto/ptr/auto-arr.hpp"

using namespace yocto;
using namespace parallel;


YOCTO_UNIT_TEST_IMPL(split)
{
    size_t Nx = 100;
    size_t Ny = 100;
    if(argc>1)
    {
        Nx = strconv::to<size_t>(argv[1],"Nx");
    }

    if(argc>2)
    {
        Ny = strconv::to<size_t>(argv[2],"Ny");
    }

    std::cerr << "sizeof(patch1D)=" << sizeof(patch1D) << std::endl;
    std::cerr << "sizeof(patch2D)=" << sizeof(patch2D) << std::endl;
    std::cerr << "sizeof(patch3D)=" << sizeof(patch3D) << std::endl;

    std::cerr << "\t(*) in 1D" << std::endl;
    patch1D p1(1,Nx);
    patch1D p1b(p1);
    std::cerr << "p1 =" << p1  << std::endl;
    std::cerr << "p1b=" << p1b << std::endl;

    for(size_t size=1;size<=10;++size)
    {
        std::cerr << std::endl << "#user_size=" << size << std::endl;

        const split::in1D s1(size,p1);
        std::cerr << "#CORE=" << s1.cores << std::endl;
        for(size_t rank=0;rank<s1.cores;++rank)
        {
            std::cerr << "\t" << size << "." << rank << ":";
            const patch1D sub = s1(rank);
            std::cerr << sub;
            std::cerr << std::endl;
        }

        slots_of<patch1D> patches;
        setup_patches(patches,size,p1);
        std::cerr << "#PATCH=" << patches.size << std::endl;
        for(size_t i=0;i<patches.size;++i)
        {
            std::cerr << "\t" << size << "." << i  << " : " << patches[i] << std::endl;
        }

    }


    std::cerr << "\t(*) in 2D" << std::endl;
    patch2D p2( coord2D(1,1), coord2D(Nx,Ny) );
    patch2D p2b(p2);

    std::cerr << "p2 =" << p2   << std::endl;
    std::cerr << "p2b=" << p2b  << std::endl;

    for(size_t size=1;size<=10;++size)
    {
        std::cerr << std::endl << "#user_size=" << size << std::endl;
        const split::in2D s2(size,p2);
        std::cerr << "\txsize=" << s2.xsize << ", ysize=" << s2.ysize << ", #cores = " << s2.cores << ", THETA = " << s2.timing << std::endl;
        for(size_t rank=0;rank<s2.cores;++rank)
        {
            std::cerr << "\t" << size << "." << rank << ":";
            const patch2D sub = s2(rank);
            std::cerr << sub;
            std::cerr << std::endl;
        }

        slots_of<patch2D> patches;
        setup_patches(patches,size,p2);
        std::cerr << "#PATCH=" << patches.size << std::endl;
        for(size_t i=0;i<patches.size;++i)
        {
            std::cerr << "\t" << size << "." << i  << " : " << patches[i] << std::endl;
        }

    }

#if 0
    std::cerr << "Split all at once..." << std::endl;
    for(size_t size=1;size<=10;++size)
    {
        std::cerr << std::endl << "#user_size=" << size << std::endl;
        auto_arr<Box> box(size);
        basic_splitV(size,size_t(1),Nx,box);
        for(size_t i=0;i<size;++i)
        {
            std::cerr << "box[" << i << "] : " << box[i].start << " -> " << box[i].final << ", count=" << box[i].count << std::endl;
        }
    }
#endif

}
YOCTO_UNIT_TEST_DONE()



