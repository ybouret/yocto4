#ifndef YOCTO_GFX_OPS_EDGES_INCLUDED
#define YOCTO_GFX_OPS_EDGES_INCLUDED 1

#include "yocto/gfx/ops/stencil.hpp"

namespace yocto
{
    namespace gfx
    {

        class Edges : public pixmap<float>
        {
        public:
            static const vertex shift[9];
            static const float  sobel1[9];
            static const float  sobel2[9];

            YOCTO_PAIR_DECL(YOCTO_TUPLE_STANDARD,info,float,Emax,float,Smax);
            info() throw() : Emax(0), Smax(0) {}
            YOCTO_PAIR_END();

        private:
            pixmap<float> I; //!< internal intensity map
        public:
            pixmap<float> S; //!< variance map

            explicit Edges(const unit_t W,const unit_t H);
            virtual ~Edges() throw();

            template <typename T>
            void build_from(const pixmap<T> &source,
                            xpatches        &xps)
            {
                // build intensity map
                src = &source;
                xps.submit(this, & Edges::buildI<T> );

                // build edges indicator
                YGFX_SUBMIT(this,&Edges::buildE,xps,xp.make<info>());

                // get amplitudes
                global.Emax = xps[1].as<info>().Emax;
                global.Smax = xps[1].as<info>().Smax;
                for(size_t i=xps.size();i>1;--i)
                {
                    const info &local = xps[i].as<info>();
                    global.Emax = max_of(global.Emax,local.Emax);
                    global.Smax = max_of(global.Smax,local.Smax);
                }

                // normalize
                if(global.Emax>0)
                {
                    xps.submit(this, & Edges::finalize );
                }
                else
                {
                    ldz();
                    S.ldz();
                }
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Edges);
            const void *src;
            info        global;

            //! build intensity by patch
            template <typename T>
            inline void buildI(xpatch &xp, lockable &) throw()
            {
                assert(src);
                const pixmap<T> &source = *static_cast<const pixmap<T>*>(src);
                for(unit_t y=xp.upper.y;y>=xp.lower.y;--y)
                {
                    for(unit_t x=xp.upper.x;x>=xp.lower.x;--x)
                    {
                        I[y][x] = pixel<T>::to_float( source[y][x] );
                    }
                }
            }

            //! build edge representation
            void buildE( xpatch &xp, lockable & ) throw();

            //! normalize
            void finalize(xpatch &xp, lockable & ) throw();
        };


        class EdgeDetector : public pixmap<float>
        {
        public:

            virtual ~EdgeDetector() throw();
            explicit EdgeDetector(const unit_t W,const unit_t H);

            pixmap<float> A;

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
                //xps.submit(this, & EdgeDetector::buildI<T> );
                YGFX_SUBMIT(this, & EdgeDetector::build<T>, xps, xp.make<float>() = 0);
                Gmax = xps[1].as<float>();
                for(size_t i=xps.size();i>1;--i)
                {
                    Gmax = max_of(Gmax,xps[i].as<float>());
                }

                if(Gmax>0)
                {
                    xps.submit(this, &EdgeDetector::normalize);
                }
                else
                {
                    ldz();
                }

            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(EdgeDetector);
            const void    *src;
            const stencil *ddx;
            const stencil *ddy;
            float          Gmax;

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
                        const float Gx = gx.dot(source,v);
                        const float Gy = gy.dot(source,v);
                        const float G  = sqrtf( Gx*Gx + Gy*Gy );
                        if(G>localGmax)
                        {
                            localGmax = G;
                        }
                        self[v] = G;
                        A[v]    = math::Atan2(Gy,Gx);
                    }
                }

                xp.as<float>() = localGmax;
            }

            void normalize(xpatch &xp, lockable &) throw();

        };

    }
}

#endif
