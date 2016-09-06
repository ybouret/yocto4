#ifndef YOCTO_GFX_OPS_EDGES_INCLUDED
#define YOCTO_GFX_OPS_EDGES_INCLUDED 1

#include "yocto/gfx/ops/stencil.hpp"
#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/gfx/ops/histogram.hpp"
#include "yocto/gfx/ops/particles.hpp"
#include "yocto/math/trigconv.hpp"

namespace yocto
{
    namespace gfx
    {


        class EdgeDetector : public pixmap<float>
        {
        public:
            static const uint8_t DIR_VERT       = 1;
            static const uint8_t DIR_DIAG_RIGHT = 2;
            static const uint8_t DIR_HORZ       = 3;
            static const uint8_t DIR_DIAG_LEFT  = 4;

            static const uint8_t STRONG = 255;
            static const uint8_t WEAK   = 127;
            
            virtual ~EdgeDetector() throw();
            explicit EdgeDetector(const unit_t W,const unit_t H);

            pixmap<uint8_t> A; //!< direction/angles
            pixmap<uint8_t> E; //!< Edge map
            
            template <typename T>
            void build_from(const pixmap<T> &source,
                            const stencil   &gx,
                            const stencil   &gy,
                            xpatches        &xps)
            {
                // build intensity map
                src = &source;
                ddx = &gx;
                ddy = &gy;
                YGFX_SUBMIT(this, & EdgeDetector::build<T>, xps, xp.make<float>() = 0);

                // then apply algorithm
                post_build(xps);
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(EdgeDetector);
            const void    *src;
            const stencil *ddx;
            const stencil *ddy;
        public:
            float          Gmax;
        private:
            Histogram      H;
            size_t         level_up;
            size_t         level_lo;
        public:
            tagmap         tags;
            particles      edges;

        private:
            void post_build(xpatches &);
            
            //! build intensity by patch
            template <typename T>
            inline void build(xpatch &xp, lockable &) throw()
            {
                assert(src);
                assert(ddx);
                assert(ddy);
                const stencil   &gx        = *ddx;
                const stencil   &gy        = *ddy;
                const pixmap<T> &source    = *static_cast<const pixmap<T>*>(src);
                pixmap<float>   &self      = *this;
                float            localGmax = 0;
                vertex v;
                unit_t &x = v.x;
                unit_t &y = v.y;

                for(y=xp.upper.y;y>=xp.lower.y;--y)
                {
                    for(x=xp.upper.x;x>=xp.lower.x;--x)
                    {
                        static const float a22_5  = math::Deg2Rad(22.5f);
                        static const float a67_5  = math::Deg2Rad(67.5f);
                        static const float a112_5 = math::Deg2Rad(112.5f);
                        static const float a157_5 = math::Deg2Rad(157.5f);
                        const float Gx = gx.dot(source,v);
                        const float Gy = gy.dot(source,v);
                        const float G  = sqrtf( Gx*Gx + Gy*Gy );
                        if(G>localGmax)
                        {
                            localGmax = G;
                        }
                        self[v]    = G;
                        uint8_t &d = A[v];
                        if(G<=0)
                        {
                            d=0; continue;
                        }
                        const float a  = math::Atan2(Gy,Gx);
                        const float aa = math::Fabs(a);

                        if(aa<=a22_5||aa>=a157_5)
                        {
                            d=DIR_VERT; continue; // VERTICAL
                        }

                        if(aa>=a67_5&&aa<=a112_5)
                        {
                            d=DIR_HORZ; continue; // HORIZONTAL
                        }

                        if( (a>=a22_5&&a<=a67_5) || (a<=-a112_5&&a>=-a157_5) )
                        {
                            d=DIR_DIAG_RIGHT; continue; // RIGHT DIAGONAL
                        }

                        d=DIR_DIAG_LEFT; // LEFT DIAGONAL
                    }
                }

                xp.as<float>() = localGmax;
            }

            void normalize(xpatch &xp, lockable &) throw();
            void non_maxima_suppress(xpatch &xp, lockable &) throw();
            void apply_thresholds(xpatch &xp, lockable &) throw();
        };

    }
}

#endif
