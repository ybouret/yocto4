#include "yocto/fame/split/quad1d.hpp"
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
            // split
            //__________________________________________________________________
            core::mpi_split(rank, size, offset, length);
            assert(length>0);
            
            //__________________________________________________________________
            //
            // building links
            //__________________________________________________________________
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
            
            return layout1D(offset,(offset+length)-1);
        }
        
    }
    
}

#include "yocto/fame/split/quad-build-ghosts.hpp"

namespace yocto
{
    
    namespace fame
    {
        
        layout1D quad1D:: outline_of(const layout1D     &sub,
                                     const int           rank,
                                     const quad_links   &links,
                                     const int           ng,
                                     local_ghosts::list &lg)
        {
            lg.clear();
            if(ng<=0)
            {
                // shouldn't be...
                return sub;
            }
            else
            {
                assert(ng>0);
                const int gshift = ng-1;
                
                //______________________________________________________________
                //
                // first pass, compute outline
                //______________________________________________________________
                unit_t lower = sub.lower;
                unit_t upper = sub.upper;
                
                if(ng>sub.width)
                {
                    throw libc::exception( EDOM, "not enough width for quad1d::outline");
                }
                
                
                unit_t source_upper[2] = {0,0};
                unit_t source_lower[2] = {0,0};
                
                unit_t target_lower[2] = {0,0};
                unit_t target_upper[2] = {0,0};
                
                for(int i=0;i<links.count;++i)
                {
                    const quad_link &link  = links[i];
                    //const bool       local = link.rank == rank;
                    switch(link.pos)
                    {
                        case quad_link::is_prev:
                        {
                            lower -= ng;
                            
                            target_lower[i] = lower;
                            target_upper[i] = lower + gshift;
                            
                            source_upper[i] = sub.upper;
                            source_lower[i] = sub.upper - gshift;
                            
                        } break;
                            
                        case quad_link::is_next:
                        {
                            upper += ng;
                            
                            target_upper[i] = upper;
                            target_lower[i] = upper - gshift;
                            
                            source_lower[i] = sub.lower;
                            source_upper[i] = sub.lower + gshift;
                            
                        } break;
                    }
                }
                const  layout1D outline(lower,upper);
                //______________________________________________________________
                //
                // second pass, compute ghosts
                //______________________________________________________________
                for(int i=0;i<links.count;++i)
                {
                    const quad_link &link  = links[i];
                    const bool       local = link.rank == rank;
                    if(local)
                    {
                        //______________________________________________________
                        //
                        // exchange data for pbc
                        //______________________________________________________
                        const layout1D source_layout(source_lower[i],source_upper[i]);
                        const layout1D target_layout(target_lower[i],target_upper[i]);
                        std::cerr << "new local ghost: " << source_layout << " --> " << target_layout << std::endl;
                        lg.push_back( new local_quad_ghosts<1>(outline,source_layout,target_layout));
                    }
                    else
                    {
                        
                    }
                }
                
                
                return outline;
            }
        }
        
    }
    
}