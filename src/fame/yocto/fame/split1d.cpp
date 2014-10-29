#include "yocto/fame/split.hpp"
#include "yocto/core/mpi-split.hpp"
#include "yocto/exceptions.hpp"

#include <cerrno>

namespace yocto
{
    namespace fame
    {
        
        static inline
        int __get_prev( int rank, int size ) throw()
        {
            assert(size>0);
            assert(rank>=0);
            assert(rank<size);
            return (rank <= 0) ? (size-1) : rank-1;
        }
        
        static inline
        int __get_next( int rank, int size ) throw()
        {
            assert(size>0);
            assert(rank>=0);
            assert(rank<size);
            return (rank>=size-1) ? 0 : rank+1;
        }
        
        
        
        layout1D Split(const layout1D &full_layout,
                       int             rank,
                       int             size,
                       bool            pbc,
                       Links          &links,
                       int            *ranks)
        {
            
            assert(rank>=0);
            assert(size>0);
            links.reset();
            if(size>full_layout.items)
            {
                throw libc::exception( EDOM, "layout is too small");
            }
            if(ranks)
            {
                ranks[0] = rank;
            }
            unit_t offset = full_layout.lower;
            unit_t length = full_layout.items;
            assert(length>0);
            core::mpi_split(rank, size, offset, length);
            if(rank==0)
            {
                if(pbc)
                {
                    links.append(__get_prev(rank,size), Link::Prev);
                }
                links.append(__get_next(rank, size), Link::Next);
            }
            else
            {
                if(rank==size-1)
                {
                    links.append(__get_prev(rank,size),Link::Prev);
                    if(pbc)
                    {
                        links.append(__get_next(rank,size),Link::Next);
                    }
                }
                else
                {
                    links.append(__get_prev(rank,size),Link::Prev);
                    links.append(__get_next(rank,size),Link::Next);
                }
            }
            
            return layout1D(offset,(offset+length)-1);
        }
        
    }
}
