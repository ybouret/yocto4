#include "yocto/gfx/ops/edges.hpp"
#include "yocto/sort/remove-if.hpp"

namespace yocto
{
    namespace gfx
    {
        EdgeDetector:: ~EdgeDetector() throw()
        {
        }

        EdgeDetector:: EdgeDetector(const unit_t W, const unit_t H) :
        pixmap<float>(W,H),
        A(w,h),
        E(w,h),
        src(0),
        ddx(0),
        ddy(0),
        Gmax(0),
        H(),
        level_up(0),
        level_lo(0),
        tags(w,h),
        edges()
        {
        }

        void EdgeDetector::normalize(xpatch &xp, lockable & ) throw()
        {
            assert(Gmax>0);
            pixmap<float> &G   = *this;
            const float    fac = 1.0f/Gmax;
            for(unit_t y=xp.upper.y;y>=xp.lower.y;--y)
            {
                pixmap<float>::row &Gy = G[y];
                for(unit_t x=xp.upper.x;x>=xp.lower.x;--x)
                {
                    Gy[x] = min_of(1.0f,fac*Gy[x]);
                }
            }
        }

        static inline bool __is_empty_particle( const particle::ptr &p ) throw()
        {
            return (p->size<=0);
        }

        void EdgeDetector:: post_build(xpatches &xps)
        {
            edges.free();

            // get the global max
            Gmax = xps[1].as<float>();
            for(size_t i=xps.size();i>1;--i)
            {
                Gmax = max_of(Gmax,xps[i].as<float>());
            }


            if(Gmax>0)
            {
                //! normalize as probability
                xps.submit(this, &EdgeDetector::normalize );

                //! non local maxima suprresion
                xps.submit(this, &EdgeDetector::non_maxima_suppress);

                //! thresholds computation
                H.reset();
                H.update(E,xps);
                level_up = H.threshold();
                const size_t level_min   = level_lo/2;
                const size_t level_delta = size_t( floor( (level_up-level_min) * weak_intake + 0.5f) );
                level_lo = level_up-level_delta;

                //! apply threshold
                xps.submit(this, &EdgeDetector::apply_thresholds);

                //! build blobs
                tags.build(E,8);

                //! find individual edges
                edges.load(tags);


                // removing weak only edges
                pixmap<float> &G = *this;
                for(size_t i=edges.size();i>0;--i)
                {
                    particle &p = *edges[i];
                    assert(p.size>0);
                    assert(p.inside.size<=0);
                    assert(p.border.size<=0);
                    // detect if at least one strong connected edge
                    bool is_strong = false;
                    for(const vnode *node = p.head;node;node=node->next)
                    {
                        if(STRONG==E[node->vtx])
                        {
                            is_strong=true;
                            break;
                        }
                    }

                    // remove it !
                    if(!is_strong)
                    {
                        while(p.size)
                        {
                            vnode       *node = p.pop_back();
                            const vertex q    = node->vtx;

                            tags[q] = 0;
                            E[q]    = 0;
                            G[q]    = 0;

                            delete node;
                        }
                    }
                }
                remove_if(edges,__is_empty_particle);
                //std::cerr << "#edges=" << edges.size() << std::endl;
            }
            else
            {
                ldz();
                E.ldz();
                tags.ldz();
            }

        }


        void EdgeDetector:: non_maxima_suppress(xpatch &xp,  lockable &access) throw()
        {
            assert(Gmax>0);
            pixmap<float> &G   = *this;
            for(unit_t y=xp.upper.y;y>=xp.lower.y;--y)
            {
                const bitmap::position_type ypos = y_position(y);
                unit_t                      yu   = y+1;
                unit_t                      yl   = y-1;
                switch(ypos)
                {
                    case bitmap::at_lower: yl = y; break;
                    case bitmap::at_upper: yu = y; break;
                    default:
                        break;
                }

                for(unit_t x=xp.upper.x;x>=xp.lower.x;--x)
                {
                    const bitmap::position_type xpos = x_position(x);
                    const float                 G0   = G[y][x];
                    bool                        keep = false;
                    unit_t                      xu   = x+1;
                    unit_t                      xl   = x-1;

                    switch(xpos)
                    {
                        case bitmap::at_lower: xl=x; break;
                        case bitmap::at_upper: xu=x; break;
                        default:
                            break;
                    }

                    switch(A[y][x])
                    {
                        case DIR_VERT:       keep = ( G[yu][x] <=G0 && G[yl][x] <=G0); break;
                        case DIR_HORZ:       keep = ( G[y][xu] <=G0 && G[y][xl] <=G0); break;
                        case DIR_DIAG_RIGHT: keep = ( G[yu][xu]<=G0 && G[yl][xl]<=G0); break;
                        case DIR_DIAG_LEFT:  keep = ( G[yl][xu]<=G0 && G[yu][xl]<=G0); break;
                        default:
                            break;
                    }

                    if(keep)
                    {
                        const uint8_t px = gist::float2byte(G0);
                        E[y][x] = px;
                    }
                    else
                    {
                        E[y][x] = 0;
                    }
                }
            }
        }


        void EdgeDetector:: apply_thresholds(xpatch &xp, lockable &access) throw()
        {
            for(unit_t y=xp.upper.y;y>=xp.lower.y;--y)
            {
                pixmap<uint8_t>::row &Ey = E[y];
                pixmap<float>::row   &Gy = (*this)[y];
                for(unit_t x=xp.upper.x;x>=xp.lower.x;--x)
                {

                    const uint8_t v = Ey[x];
                    if(v<=level_lo)
                    {
                        Gy[x] = 0;
                        Ey[x] = 0;
                        continue;
                    }

                    if(v<=level_up)
                    {
                        Ey[x] = WEAK;
                        continue ;
                    }

                    Ey[x] = STRONG;
                }
            }
        }


    }
}
