#include "yocto/core/mpi-split.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

namespace {
    
    template <typename T>
    static inline
    void __test_split()
    {
        const T global_offset(1);
        const T global_length(100);
    
        for(size_t size=1;size<=6;++size)
        {
            std::cerr << "size=" << size << ": " << global_offset << " -> " << global_length+global_offset-1  << std::endl;
            for(size_t rank=0;rank<size;++rank)
            {
                std::cerr << "\t" << size << "." << rank << ": ";
                T offset = global_offset;
                T length = global_length;
                core::mpi_split(rank, size, offset, length);
                std::cerr << offset << " -> " << length + offset -1 << " / length=" <<  length << std::endl;
                std::cerr << std::endl;
            }
        }
    }

    template <typename T>
    static inline
    void __test_split2()
    {
        const T global_offset(1);
        const T global_xlength(100);
        const T global_ylength(200);

        for(size_t size=1;size<=8;++size)
        {
            std::cerr << std::endl;
            core::mpi_split2D<T> split(size,global_xlength,global_ylength);
            for(size_t rank=0;rank<size;++rank)
            {
                std::cerr << "\t" << size << "." << rank << ": ";
                T xoffset = global_offset;
                T yoffset = global_offset;
                T xlength = global_xlength;
                T ylength = global_ylength;
                split(rank, xoffset, yoffset, xlength, ylength);
                std::cerr << " x: "  << xoffset << "->" << (xoffset+xlength-1);
                std::cerr << "| y: " << yoffset << "->" << (yoffset+ylength-1);
                std::cerr << std::endl;

            }
        }
    }
    
}


YOCTO_UNIT_TEST_IMPL(mpi_split)
{
    __test_split<int>();
    __test_split<size_t>();
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/ios/ocstream.hpp"

YOCTO_UNIT_TEST_IMPL(mpi_split2)
{
#if 0
    double Lx = 1;
    double Ly = 1;
    for(int N=1;N<=10;++N)
    {
        ios::wcstream fp( vformat("tau%d.dat",N) );
        for(int nx=1;nx<=N;++nx)
        {
            for(int ny=1;ny<=N;++ny)
            {
                if(nx*ny==N)
                {
                    const double Tau = Ly/ny + Lx/nx;
                    fp("%d %g\n", nx, Tau);
                }
            }
        }
    }
#endif
    
    __test_split2<int>();
    __test_split2<size_t>();
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/parallel/split.hpp"

using namespace parallel;

YOCTO_UNIT_TEST_IMPL(split)
{
    std::cerr << "sizeof(patch1D)=" << sizeof(patch1D) << std::endl;
    std::cerr << "sizeof(patch2D)=" << sizeof(patch2D) << std::endl;
    std::cerr << "sizeof(patch3D)=" << sizeof(patch3D) << std::endl;

    patch1D p1(1,100);
    patch1D p1b(p1);
    std::cerr << "p1 =" << p1  << std::endl;
    std::cerr << "p1b=" << p1b << std::endl;

    std::cerr << "\t(*) in 1D" << std::endl;
    for(size_t size=1;size<=8;++size)
    {
        std::cerr << std::endl << "#size=" << size << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            std::cerr << "\t" << size << "." << rank << ":";
            const patch1D sub = split::compute(rank,size,p1);
            std::cerr << sub;
            std::cerr << std::endl;
        }
    }


    patch2D p2( coord2D(1,1), coord2D(20,20) );
    patch2D p2b(p2);

    std::cerr << "p2 =" << p2   << std::endl;
    std::cerr << "p2b=" << p2b  << std::endl;

    std::cerr << "\t(*) in 2D" << std::endl;
    for(size_t size=1;size<=8;++size)
    {
        std::cerr << std::endl << "#size=" << size << std::endl;
        const split::in2D s2(size,p2);
        std::cerr << "\txsize=" << s2.xsize << ", ysize=" << s2.ysize << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            std::cerr << "\t" << size << "." << rank << ":";
            const patch2D sub = s2(rank);
            std::cerr << sub;
            std::cerr << std::endl;
        }

    }

}
YOCTO_UNIT_TEST_DONE()



