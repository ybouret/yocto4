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
                    const double Tau = nx*Ly + ny *Lx;
                    fp("%d %g\n", nx, Tau);
                }
            }
        }
    }
    __test_split2<int>();
    __test_split2<size_t>();
}
YOCTO_UNIT_TEST_DONE()

