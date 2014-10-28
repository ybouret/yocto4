#include "yocto/fame/split.hpp"
#include "yocto/core/mpi-split.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>

namespace yocto
{
    namespace fame
    {
        layout1D Link::Split(const layout1D &full_layout,
                             int rank,
                             int size )
        {
            
            assert(rank>=0);
            assert(size>0);
            if(size>1)
            {
                unit_t offset = full_layout.lower;
                unit_t length = full_layout.items;
                if(length<=0)
                    throw libc::exception( EDOM, "layout is too small");
                core::mpi_split(rank, size, offset, length);
                return layout1D(offset,(offset+length)-1);
            }
            else
            {
                return full_layout;
            }
        }
        
    }
}
