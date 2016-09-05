#ifndef YOCTO_GFX_OPS_EDGES_INCLUDED
#define YOCTO_GFX_OPS_EDGES_INCLUDED 1

#include "yocto/gfx/ops/stencil.hpp"
#include "yocto/math/trigconv.hpp"

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
            static const uint8_t DIR_VERT       = 1;
            static const uint8_t DIR_DIAG_RIGHT = 2;
            static const uint8_t DIR_HORZ       = 3;
            static const uint8_t DIR_DIAG_LEFT  = 4;

            virtual ~EdgeDetector() throw();
            explicit EdgeDetector(const unit_t W,const unit_t H);

            pixmap<uint8_t> A;
            pixmap<uint8_t> B;
            pixmap<float>   E;
            
            template <typename T>
            void build_from(const pixmap<T> &source,
                            const stencil   &gx,
                            const stencil   &gy,
                            xpatches        &xps)
            {
                // build intensity map
                std::cerr << "Build Maps" << std::endl;
                src = &source;
                ddx = &gx;
                ddy = &gy;
                YGFX_SUBMIT(this, & EdgeDetector::build<T>, xps, xp.make<float>() = 0);
                Gmax = xps[1].as<float>();

                // get the global max
                for(size_t i=xps.size();i>1;--i)
                {
                    Gmax = max_of(Gmax,xps[i].as<float>());
                }

                if(Gmax>0)
                {
                    std::cerr << "Non Maxima Suppress" << std::endl;
                    xps.submit(this, &EdgeDetector::non_maxima_suppress);
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
        public:
            float          Gmax;
        private:

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
                        B[v]       = 0xff;
                        if(G<=0)
                        {
                            B[v] = 0x00;
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

            //void normalize(xpatch &xp, lockable &) throw();
            void non_maxima_suppress(xpatch &xp, lockable &) throw();
            
        };

    }
}

#endif
