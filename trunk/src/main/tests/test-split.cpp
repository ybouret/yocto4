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
    
}


YOCTO_UNIT_TEST_IMPL(mpi_split)
{
    __test_split<int>();
    __test_split<size_t>();
    
}
YOCTO_UNIT_TEST_DONE()
