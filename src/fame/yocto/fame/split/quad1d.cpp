#include "yocto/fame/split/quad1d.hpp"
#include "yocto/parallel/split.hpp"
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
        
        
        layout1D quad1D::split(const layout1D &full,
                               const int       rank,
                               const int       size,
                               const bool      pbc,
                               quad_links     &links )
        {
            
            assert(size>0);
            assert(rank<size);
            links.reset();
            
            //__________________________________________________________________
            //
            // get data
            //__________________________________________________________________
            
            unit_t offset = full.lower;
            unit_t length = full.width;
            if(length<size)
            {
                throw libc::exception(EDOM,"quad_split1d: layout is too small");
            }
            
            //__________________________________________________________________
            //
            // load balancing
            //__________________________________________________________________
            parallel::split::compute1D(rank, size, offset, length);
            assert(length>0);
            
            //__________________________________________________________________
            //
            // building links
            //__________________________________________________________________
            if(size>1)
            {
                
                if(rank==0)
                {
                    if(pbc)
                    {
                        links.append(quad_link::is_prev,__get_prev(rank,size));
                    }
                    links.append(quad_link::is_next, __get_next(rank,size));
                }
                else
                {
                    if(size-1==rank)
                    {
                        links.append(quad_link::is_prev,__get_prev(rank,size));
                        if(pbc)
                        {
                            links.append(quad_link::is_next, __get_next(rank,size));
                        }
                    }
                    else
                    {
                        assert(rank>0);
                        assert(size>1);
                        assert(rank<size-1);
                        links.append(quad_link::is_prev,rank-1);
                        links.append(quad_link::is_next,rank+1);
                    }
                }
            }
            else
            {
                assert(1==size);
                assert(0==rank);
                if(pbc)
                {
                    links.append(quad_link::is_prev,0);
                    links.append(quad_link::is_next,0);
                }
            }
            
            return layout1D(offset,(offset+length)-1);
        }
        
    }
    
}

