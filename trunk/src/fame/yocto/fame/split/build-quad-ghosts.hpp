#ifndef YOCTO_FAME_SPLIT_BUILD_QUAD_GHOSTS_INCLUDED
#define YOCTO_FAME_SPLIT_BUILD_QUAD_GHOSTS_INCLUDED 1

#include "yocto/fame/split/quad-links.hpp"
#include "yocto/fame/split/quad-ghosts.hpp"

namespace yocto
{
    
    namespace fame
    {
        
        template <typename Layout>
        struct build_quad_ghosts
        {
            static const size_t DIMENSIONS = Layout::DIMENSIONS;
            
            typedef typename Layout::coord         Coord;
            typedef quad_ghosts<Layout>            Ghosts;
            typedef typename Ghosts::list          GhostsList;
            
            static inline
            Layout outline_for(const int         rank,
                               const Layout     &inside,
                               const int         ng,
                               const quad_links *links,
                               GhostsList       &lg,
                               GhostsList       &ag)
            {
                assert(links);
                lg.clear();
                if(ng<=0)
                {
                    return inside;
                }
                
                //______________________________________________________________
                //
                // First Pass: build outline
                //______________________________________________________________
                Coord lower = inside.lower;
                Coord upper = inside.upper;
                for(size_t dim=0;dim<DIMENSIONS;++dim)
                {
                    unit_t           &lo = __coord(lower,dim);
                    unit_t           &up = __coord(upper,dim);
                    const quad_links &the_links = links[dim];
                    for(size_t i=0;i<the_links.count;++i)
                    {
                        const quad_link &link = the_links[i];
                        switch(link.pos)
                        {
                            case quad_link::is_prev:
                                lo -= ng;
                                break;
                                
                            case quad_link::is_next:
                                up += ng;
                                break;
                                
                        }
                        
                    }
                }
                
                const Layout outline(lower,upper);
                const int gshift = ng-1;
                
                //______________________________________________________________
                //
                // Second Pass: build ghosts
                //______________________________________________________________
                for(size_t dim=0;dim<DIMENSIONS;++dim)
                {
                    const quad_links &the_links = links[dim];
                    for(size_t i=0;i<the_links.count;++i)
                    {
                        const quad_link &link  = the_links[i];
                        const bool       local = link.rank == rank;
                        switch(link.pos)
                        {
                            case quad_link::is_prev:
                            {
                                const unit_t target_lo    = __coord(outline.lower,dim);
                                const unit_t target_up    = target_lo + gshift;
                                const Coord  target_lower = replace_coord(target_lo, outline.lower, dim);
                                const Coord  target_upper = replace_coord(target_up, outline.upper, dim);
                                const Layout target(target_lower,target_upper);
                                //std::cerr << "\t\t" << get_axis_name(dim) << "ghost@prev: " << target << std::endl;
                                if(local)
                                {
                                    // take data from the 'next' side
                                    const unit_t source_up    = __coord(inside.upper,dim);
                                    const unit_t source_lo    = source_up - gshift;
                                    const Coord  source_lower = replace_coord(source_lo, outline.lower, dim);
                                    const Coord  source_upper = replace_coord(source_up, outline.upper, dim);
                                    const Layout source(source_lower,source_upper);
                                    //std::cerr << "\t\t+" << get_axis_name(dim) << "local <-- " << source << std::endl;
                                    lg.push_back( new Ghosts(outline,link.rank,dim,source,target) );
                                }
                                else
                                {
                                    // take data on the same side, to be sent
                                    const unit_t source_lo    = __coord(inside.lower,dim);
                                    const unit_t source_up    = source_lo + gshift;
                                    const Coord  source_lower = replace_coord(source_lo, outline.lower, dim);
                                    const Coord  source_upper = replace_coord(source_up, outline.upper, dim);
                                    const Layout source(source_lower,source_upper);
                                    //std::cerr << "\t\t+" << get_axis_name(dim) << "async <--  " << source << std::endl;
                                    ag.push_back( new Ghosts(outline,link.rank,dim,source,target));
                                }
                                
                            }   break;
                                
                            case quad_link::is_next:
                            {
                                const unit_t target_up    = __coord(outline.upper,dim);
                                const unit_t target_lo    = target_up - gshift;
                                const Coord  target_lower = replace_coord(target_lo, outline.lower, dim);
                                const Coord  target_upper = replace_coord(target_up, outline.upper, dim);
                                const Layout target(target_lower,target_upper);
                                //std::cerr << "\t\t" << get_axis_name(dim) << "ghost@next: " << target << std::endl;
                                if(local)
                                {
                                    // take data on the 'prev' side
                                    const unit_t source_lo = __coord(inside.lower,dim);
                                    const unit_t source_up = source_lo + gshift;
                                    const Coord  source_lower = replace_coord(source_lo, outline.lower, dim);
                                    const Coord  source_upper = replace_coord(source_up, outline.upper, dim);
                                    const Layout source(source_lower,source_upper);
                                    //std::cerr << "\t\t+" << get_axis_name(dim) << "local <--  " << source << std::endl;
                                    lg.push_back( new Ghosts(outline,link.rank,dim,source,target) );
                                }
                                else
                                {
                                    // take data on the same side, to be sent
                                    const unit_t source_up    = __coord(inside.upper,dim);
                                    const unit_t source_lo    = source_up - gshift;
                                    const Coord  source_lower = replace_coord(source_lo, outline.lower, dim);
                                    const Coord  source_upper = replace_coord(source_up, outline.upper, dim);
                                    const Layout source(source_lower,source_upper);
                                    //std::cerr << "\t\t+" << get_axis_name(dim) << "async <-- " << source << std::endl;
                                    ag.push_back( new Ghosts(outline,link.rank,dim,source,target) );
                                }
                            }  break;
                        }
                    }
                }
                
                return outline;
            }
            
            static inline Coord replace_coord(unit_t value, const Coord &C, size_t dim) throw()
            {
                Coord ans = C;
                __coord(ans,dim) = value;
                return ans;
            }
            
        };
        
        
    }
}

#endif
