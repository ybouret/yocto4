#ifndef YOCTO_GFX_GRADIENT_INCLUDED
#define YOCTO_GFX_GRADIENT_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/math/types.hpp"
#include "yocto/gfx/parallel.hpp"

namespace yocto
{
    namespace gfx
    {

#define YOCTO_GFX_GRADIENT_COMPUTE(VALUE)   do { \
const double gg = math::Hypotenuse(gx, gy);      \
VALUE           = gg;                            \
if(gg>Gmax) { Gmax = gg; }                       \
} while(false)

        struct gradient
        {

            class ipatch : public patch
            {
            public:
                double          Gmax;
                pixmap<double> *target;
                const void     *source;
                lockable       *access;

                explicit ipatch( const patch2D &p ) throw();
                virtual ~ipatch() throw();

                template <typename T>
                inline void inside() throw()
                {
                    assert(source);
                    assert(target);
                    Gmax = 0;
                    const pixmap<T> &data = *(const pixmap<T> *)(source);
                    pixmap<double>  &G    = *target;
                    assert(G.w==data.w);
                    assert(G.h==data.h);
                    const unit_t     xlo  = area.x;
                    const unit_t     xhi  = area.xout;
                    const unit_t     xlom = xlo-1;
                    const unit_t     xlop = xlo+1;
                    const unit_t     ylo  = area.y;
                    const unit_t     yhi  = area.yout;
#if 0
                    {
                        YOCTO_LOCK(*access);
                        std::cerr << "xlo=" << xlo << ", xhi=" << xhi-1 << std::endl;
                        std::cerr << "ylo=" << ylo << ", yhi=" << yhi-1 << std::endl;
                    }
#endif
                    for(unit_t jm=ylo-1,j=ylo,jp=ylo+1;j<yhi;++jm,++j,++jp)
                    {
#if 1
                        if(!(j>0&&j<G.h))
                        {
                            //YOCTO_LOCK(*access);
                            //YOCTO_GIANT_LOCK();
                            std::cerr << "j=" << j << ", h=" << G.h << std::endl;
                        }
#endif
                        assert(j>0);
                        assert(j<G.h);
                        pixmap<double>::row           &Gj  = G[j];
                        const typename pixmap<T>::row &dj  = data[j];
                        const typename pixmap<T>::row &djm = data[jm];
                        const typename pixmap<T>::row &djp = data[jp];

                        for(unit_t im=xlom,i=xlo,ip=xlop;i<xhi;++im,++i,++ip)
                        {
                            const double gx = double(dj[ip])-double(dj[im]);
                            const double gy = double(djp[i])-double(djm[i]);
                            continue;
                            YOCTO_GFX_GRADIENT_COMPUTE(Gj[i]);
                        }
                    }

                }


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(ipatch);
            };

            typedef dynamic_slots<ipatch> ipatches;

            inline
            static void setup(ipatches    &input,
                              const size_t cpus,
                              const bitmap &bmp)
            {
                input.free();
                patch::setup(input, cpus,bmp,false);
            }

            template <typename T> inline
            static void start(ipatches          &input,
                              pixmap<double>    &G,
                              const pixmap<T>   &data,
                              threading::server &psrv)
            {
                // compute core
                {
                    YOCTO_LOCK(psrv.access);
                    std::cerr << "Starting gradient..." << std::endl;
                }
                for(size_t i=0;i<input.size;++i)
                {
                    ipatch &p = input[i];
                    p.target  = &G;
                    p.source  = &data;
                    p.access  = &psrv.access;
                    const threading::server::job J(&p,&ipatch::inside<T> );
                    psrv.enqueue(J);
                }
                psrv.flush();
            }

            template <typename T>
            static double borders(pixmap<double>  &G,
                                  const pixmap<T> &data) throw()
            {
                double Gmax = 0;
                assert(G.w==data.w);
                assert(G.h==data.h);
                const unit_t h = G.h;
                for(unit_t jm=0,j=1,jp=2;j<h;++jm,++j,++jp)
                {
                     pixmap<double>::row           &Gj  = G[j];


                }


                return Gmax;
            }


        };

#if 0

        template <typename T>
        double gradient_of( const pixmap<T> &data, pixmap<double> &G )
        {
            assert(data.w==G.w);
            assert(data.h==G.h);
            const unit_t w    = data.w;
            const unit_t h    = data.h;
            const unit_t w0   = w-1;
            const unit_t w1   = w-2;
            const unit_t w2   = w-3;
            double       Gmax = 0.0;


            //__________________________________________________________________
            //
            // all but top and down
            //__________________________________________________________________
            for(unit_t j=1,jm=0,jp=2;jp<h;++j,++jm,++jp)
            {
                pixmap<double>::row           &Gj  = G[j];
                const typename pixmap<T>::row &dj  = data[j];
                const typename pixmap<T>::row &djm = data[jm];
                const typename pixmap<T>::row &djp = data[jp];

                //______________________________________________________________
                //
                // all but left and right side
                //______________________________________________________________
                for(unit_t i=1,im=0,ip=1;ip<w;++i,++im,++ip)
                {
                    const double gx = double(dj[ip])-double(dj[im]);
                    const double gy = double(djp[i])-double(djm[i]);
                    YOCTO_GFX_GRADIENT_COMPUTE(Gj[i]);
                }

                //______________________________________________________________
                //
                // left side
                //______________________________________________________________
                {
                    const double gy = double(djp[0])-double(djm[0]);
                    const double gx = 4.0 * double(dj[1]) - 3.0 * double(dj[0]) - double(dj[2]);
                    YOCTO_GFX_GRADIENT_COMPUTE(Gj[0]);
                }

                //______________________________________________________________
                //
                // right side
                //______________________________________________________________
                {
                    const double gy = double(djp[w0])-double(djm[w0]);
                    const double gx = 4.0 * double(dj[w1]) - 3.0 * double(dj[w0]) - double(dj[w2]);
                    YOCTO_GFX_GRADIENT_COMPUTE(Gj[w0]);
                }

            }

            //__________________________________________________________________
            //
            // top and down, but corners
            //__________________________________________________________________
            const typename pixmap<T>::row &d0  = data[0];
            pixmap<double>::row           &G0  = G[0];

            const unit_t h0 = h-1;
            const unit_t h1 = h-2;
            const unit_t h2 = h-3;
            const typename pixmap<T>::row &dh0  = data[h0];
            const typename pixmap<T>::row &dh1  = data[h1];
            const typename pixmap<T>::row &dh2  = data[h2];
            pixmap<double>::row           &Gh0  = G[h0];

            for(unit_t im=0,i=1,ip=2;ip<w;++im,++i,++ip)
            {

                // down
                {
                    const double gx = double(d0[ip]) - double(d0[im]);
                    const double gy = 4.0*double(data[0][i]) - 3.0 * double(data[1][i]) - double(data[2][i]);
                    YOCTO_GFX_GRADIENT_COMPUTE(G0[i]);
                }


                // top
                {
                    const double gx = double(dh0[ip]) - double(dh0[im]);
                    const double gy = 4.0*double(dh0[i]) - 3.0 * double(dh1[i]) - double(dh2[i]);
                    YOCTO_GFX_GRADIENT_COMPUTE(Gh0[i]);
                }
            }

            //__________________________________________________________________
            //
            // and corners...
            //__________________________________________________________________
            {
                const double gx = 4.0 * double(data[0][0]) - 3.0 * double(data[0][1]) - double(data[0][2]);
                const double gy = 4.0 * double(data[0][0]) - 3.0 * double(data[1][0]) - double(data[2][0]);
                YOCTO_GFX_GRADIENT_COMPUTE(G[0][0]);
            }

            {
                const double gx = 4.0 * double(data[0][w0]) - 3.0 * double(data[0][w1]) - double(data[0][w2]);
                const double gy = 4.0 * double(data[0][w0]) - 3.0 * double(data[1][w0]) - double(data[2][w0]);
                YOCTO_GFX_GRADIENT_COMPUTE(G[0][w0]);
            }

            {
                const double gx = 4.0 * double(data[h0][0]) - 3.0 * double(data[h0][1]) - double(data[h0][2]);
                const double gy = 4.0 * double(data[h0][0]) - 3.0 * double(data[h1][0]) - double(data[h2][0]);
                YOCTO_GFX_GRADIENT_COMPUTE(G[h0][0]);
            }

            {
                const double gx = 4.0 * double(data[h0][w0]) - 3.0 * double(data[h0][w1]) - double(data[h0][w2]);
                const double gy = 4.0 * double(data[h0][w0]) - 3.0 * double(data[h1][w0]) - double(data[h2][w0]);
                YOCTO_GFX_GRADIENT_COMPUTE(G[h0][w0]);
            }
            return Gmax;
        }

        template <typename T>
        void gradient( pixmap<T> &grad, const pixmap<T> &data ) throw()
        {

            assert(grad.w == data.w );
            assert(grad.h == data.h );
            const unit_t w = data.w;
            const unit_t h = data.h;

            // first pass: build 'real' gradient
            pixmap<double> G(w,h);
            const double Gmax = gradient_of(data, G);

            // second pass: convert to type
            if(Gmax<=0)
            {
                grad.ldz();
            }
            else
            {
                for(unit_t j=0;j<h;++j)
                {
                    typename pixmap<T>::row         &gj  = grad[j];
                    const pixmap<double>::row       &Gj  = G[j];

                    for(unit_t i=0;i<w;++i)
                    {
                        gj[i] = to_unit<T>(Gj[i]/Gmax);
                    }
                }
            }
        }
#endif

#if 0
        template <typename T>
        class gradient_patch : public ipatch
        {
        public:
            double Gmax;

            explicit gradient_patch( const rectangle &r ) throw() :
            ipatch(r),
            Gmax(0)
            {
            }

            virtual ~gradient_patch() throw() {}


            static inline
            void call1( threading::SIMD::Context &ctx ) throw()
            {
                gradient_patch &self = ctx.as<gradient_patch>();
                {
                    YOCTO_LOCK(ctx.access);
                    std::cerr << "$1@ctx#" << ctx.rank << "[" << self.rect.x << ":" << self.rect.xout-1 << "] x [" << self.rect.y << ":" << self.rect.yout-1 << "]" << std::endl;
                    }
                    self.compute1();
                    }

                    static inline
                    void call2(threading::SIMD::Context &ctx ) throw()
                    {
                        gradient_patch &self = ctx.as<gradient_patch>();
                        {
                            YOCTO_LOCK(ctx.access);
                            std::cerr << "$2@ctx#" << ctx.rank << "[" << self.rect.x << ":" << self.rect.xout-1 << "] x [" << self.rect.y << ":" << self.rect.yout-1 << "]" << std::endl;
                        }
                        self.compute2();
                    }

                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(gradient_patch);
                    inline void compute1() throw()
                    {
                        assert(source);
                        assert(target);
                        Gmax = 0;
                        const pixmap<T> &data = *(const pixmap<T> *)(source);
                        pixmap<double>  &G    = *(pixmap<double>  *)(target);
                        const unit_t     xlo  = rect.x;
                        const unit_t     xhi  = rect.xout;
                        const unit_t     xlom = xlo-1;
                        const unit_t     xlop = xlo+1;
                        const unit_t     ylo  = rect.y;
                        const unit_t     yhi  = rect.yout;

                        for(unit_t jm=ylo-1,j=ylo,jp=ylo+1;j<yhi;++jm,++j,++jp)
                        {
                            pixmap<double>::row           &Gj  = G[j];
                            const typename pixmap<T>::row &dj  = data[j];
                            const typename pixmap<T>::row &djm = data[jm];
                            const typename pixmap<T>::row &djp = data[jp];

                            for(unit_t im=xlom,i=xlo,ip=xlop;i<xhi;++im,++i,++ip)
                            {
                                const double gx = double(dj[ip])-double(dj[im]);
                                const double gy = double(djp[i])-double(djm[i]);
                                YOCTO_GFX_GRADIENT_COMPUTE(Gj[i]);
                            }
                        }
                    }

                    inline void compute2() throw()
                    {
                        assert(source);
                        assert(target);
                        assert(params);
                        const double          den  = *(const double *)params;
                        const pixmap<double> &G    = *(const pixmap<double> *)(source);
                        pixmap<T>            &grad = *(pixmap<T>            *)(target);
                        const unit_t          xlo  = rect.x;
                        const unit_t          xhi  = rect.xout;
                        const unit_t          ylo  = rect.y;
                        const unit_t          yhi  = rect.yout;
                        for(unit_t j=ylo;j<yhi;++j)
                        {
                            typename pixmap<T>::row            &gj = grad[j];
                            const typename pixmap<double>::row &Gj = G[j];
                            for(unit_t i=xlo;i<xhi;++i)
                            {
                                gj[i] = to_unit<T>(Gj[i]/den);
                            }
                        }
                    }
                    };


                    template <typename T>
                    double gradient_of( const pixmap<T> &data, pixmap<double> &G, threading::SIMD &simd)
                    {
                        assert(data.w==G.w);
                        assert(data.h==G.h);
                        const unit_t w    = data.w;
                        const unit_t h    = data.h;

                        // prepare inside blocks
                        setup_contexts< gradient_patch<T> >(simd,w,h,false);
                        reset_contexts< gradient_patch<T> >(simd, &data, &G, NULL);
                        threading::SIMD::Kernel K( cfunctor(gradient_patch<T>::call1) );
                        
                        // compute inside blocks
                        simd(K);
                        
                        double Gmax = 0;
                        
                        // compute borders
                        
                        
                        // compute final Gmax
                        const size_t size = simd.size;
                        for(size_t rank=0;rank<size;++rank)
                        {
                            const double tmp  = simd.get< gradient_patch<T> >(rank).Gmax;
                            if(tmp>Gmax) Gmax = tmp;
                        }
                        return Gmax;
                    }
                    
                    template <typename T>
                    void gradient( pixmap<T> &grad, const pixmap<T> &data, threading::SIMD &simd) throw()
                    {
                        
                        assert(grad.w == data.w );
                        assert(grad.h == data.h );
                        const unit_t w = data.w;
                        const unit_t h = data.h;
                        
                        // first pass: build 'real' gradient
                        pixmap<double> G(w,h);
                        const double   Gmax = gradient_of(data, G,simd);
                        if(Gmax<=0)
                        {
                            grad.ldz();
                        }
                        else
                        {
                            setup_contexts< gradient_patch<T> >(simd,w,h,true);
                            reset_contexts< gradient_patch<T> >(simd, &G, &grad, &Gmax);
                            threading::SIMD::Kernel K( cfunctor(gradient_patch<T>::call2) );
                            simd(K);
                        }
                        
                    }
#endif
                    
                    }
                    
                    }
                    
#endif
