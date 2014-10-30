#include "yocto/fame/split/quad2d.hpp"
#include "yocto/fame/split/quad1d.hpp"

#include <cmath>

namespace yocto
{
    
    namespace fame
    {
        
        static inline
        void find_sizes( coord2D &sizes, const unit_t Nx, const unit_t Ny,  const int size )
        {
            const double target = Nx/double(Ny);
            double       dopt   = -1;
            int          xopt   = -1;
            int          yopt   = -1;
            
            for(int px=1;px<=size;++px)
            {
                for(int py=1;py<=size;++py)
                {
                    if(px*py==size)
                    {
                        const double ratio = px/double(py);
                        const double dtmp  = fabs(target-ratio);
                        if(dopt<0 || dtmp<dopt)
                        {
                            xopt = px;
                            yopt = py;
                            dopt = dtmp;
                        }
                    }
                }
            }
            sizes.x = xopt;
            sizes.y = yopt;
            
        }
        
        layout2D quad2D:: split(const layout2D &full,
                                const int       rank,
                                const int       size,
                                const bool      xpbc,
                                quad_links     &xlinks,
                                const bool      ypbc,
                                quad_links     &ylinks,
                                coord2D        &ranks,
                                coord2D        &sizes)
        {
            
            assert(size>0);
            assert(rank<size);
            
            const unit_t Nx = full.width.x;
            const unit_t Ny = full.width.y;
            
            find_sizes(sizes,Nx,Ny,size);
            
            //__________________________________________________________________
            //
            // rank = nx * ry + rx
            //__________________________________________________________________
            const int nx = sizes.x;
            const int ny = sizes.y;
            
            const int rx = rank % nx;
            const int ry = rank / nx;
            assert(nx*ry+rx==rank);
            ranks.x = rx;
            ranks.y = ry;
            
            //__________________________________________________________________
            //
            // 1D X split
            //__________________________________________________________________
            const layout1D xfull(full.lower.x,full.upper.x);
            const layout1D xsub = quad1D::split(xfull, rx, nx, xpbc, xlinks);
            for(size_t i=0;i<xlinks.count;++i)
            {
                xlinks[i].rank = nx * ry + xlinks[i].rank;
            }
            
            //__________________________________________________________________
            //
            // 1D Y split
            //__________________________________________________________________
            const layout1D yfull(full.lower.y,full.upper.y);
            const layout1D ysub = quad1D::split(yfull, ry, ny, ypbc, ylinks);
            for(size_t i=0;i<ylinks.count;++i)
            {
                ylinks[i].rank = nx * ylinks[i].rank + rx;
            }
            
            return layout2D( coord2D(xsub.lower,ysub.lower), coord2D(xsub.upper,ysub.upper) );
            
        }
        
        layout2D quad2D:: outline_of(const layout2D     &sub,
                                     const int           rank,
                                     const quad_links   &xlinks,
                                     const quad_links   &ylinks,
                                     const int           ng,
                                     local_ghosts::list &lg)
        {
            lg.clear();
            
            if(ng<=0)
            {
                return sub;
            }
            else
            {
                assert(ng>0);
                
                const int gshift = ng-1;
                
                //______________________________________________________________
                //
                // First pass: compute outline
                //______________________________________________________________
                coord2D lower = sub.lower;
                coord2D upper = sub.upper;
                
                for(size_t i=0;i<xlinks.count;++i)
                {
                    const quad_link &link = xlinks[i];
                    switch(link.pos)
                    {
                        case quad_link::is_prev:
                            lower.x -= ng;
                            break;
                            
                        case quad_link::is_next:
                            upper.x += ng;
                            break;
                    }
                }
                
                for(size_t j=0;j<ylinks.count;++j)
                {
                    const quad_link &link = xlinks[j];
                    switch(link.pos)
                    {
                        case quad_link::is_prev:
                            lower.y -= ng;
                            break;
                            
                        case quad_link::is_next:
                            upper.y += ng;
                            break;
                    }
                }
                
                const layout2D outline(lower,upper);
                
                //______________________________________________________________
                //
                // second pass: compute ghosts
                //______________________________________________________________
                for(size_t i=0;i<xlinks.count;++i)
                {
                    const quad_link &link  = xlinks[i];
                    const bool       local = (link.rank == rank);
                    const unit_t     ylo   = outline.lower.y;
                    const unit_t     yup   = outline.upper.y;
                    switch(link.pos)
                    {
                        case quad_link::is_prev:
                        {
                            const unit_t target_xlo = outline.lower.x;
                            const unit_t target_xup = target_xlo + gshift;
                            if(local)
                            {
                                const unit_t source_xup = sub.upper.x;
                                const unit_t source_xlo = source_xup - gshift;
                                const layout2D source_layout( coord2D(source_xlo,ylo), coord2D(source_xup,yup));
                                const layout2D target_layout( coord2D(target_xlo,ylo), coord2D(target_xup,yup));
                                std::cerr << "new Xlocal2D : " << source_layout << " --> " << target_layout << std::endl;
                                lg.push_back( new local_ghosts(outline,source_layout,target_layout) );
                            }
                            else
                            {
                                
                            }
                        }   break;
                            
                        case quad_link::is_next:
                        {
                            const unit_t target_xup = outline.upper.x;
                            const unit_t target_xlo = target_xup - gshift;
                            if(local)
                            {
                                const unit_t source_xlo = sub.lower.x;
                                const unit_t source_xup = source_xlo + gshift;
                                const layout2D source_layout( coord2D(source_xlo,ylo), coord2D(source_xup,yup));
                                const layout2D target_layout( coord2D(target_xlo,ylo), coord2D(target_xup,yup));
                                std::cerr << "new Xlocal2D : " << source_layout << " --> " << target_layout << std::endl;
                                lg.push_back( new local_ghosts(outline,source_layout,target_layout) );
                            }
                            else
                            {
                                
                            }
                        } break;
                    }
                }
                
                
                for(size_t j=0;j<ylinks.count;++j)
                {
                    const quad_link &link  = xlinks[j];
                    const bool       local = (link.rank == rank);
                    const unit_t     xlo   = outline.lower.x;
                    const unit_t     xup   = outline.upper.x;
                    
                    switch(link.pos)
                    {
                        case quad_link::is_prev:
                        {
                            const unit_t target_ylo = outline.lower.y;
                            const unit_t target_yup = target_ylo + gshift;
                            if( local)
                            {
                            }
                        } break;
                            
                        case quad_link::is_next:
                        {
                            
                        } break;
                    }
                }
                
                
                
                
                
                
                return outline;
            }
            
        }
        
        
    }
    
}


