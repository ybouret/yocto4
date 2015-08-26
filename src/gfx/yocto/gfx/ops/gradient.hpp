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

                explicit ipatch( const patch2D &p ) throw();
                virtual ~ipatch() throw();

                template <typename T>
                inline void inside(lockable &access) throw()
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
                        YOCTO_LOCK(access);
                        std::cerr << "xlo=" << xlo << ", xhi=" << xhi-1 << std::endl;
                        std::cerr << "ylo=" << ylo << ", yhi=" << yhi-1 << std::endl;
                    }
#endif
                    for(unit_t jm=ylo-1,j=ylo,jp=ylo+1;j<yhi;++jm,++j,++jp)
                    {
#if 0
                        if(!(j>0&&j<G.h))
                        {

                            YOCTO_LOCK(access);
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
                    const threading::server::job J(&p,&ipatch::inside<T>);
                    psrv.enqueue(J);
                }
            }

            template <typename T> inline
            static double borders(pixmap<double>  &G,
                                  const pixmap<T> &data) throw()
            {
                double Gmax = 0;
                assert(G.w==data.w);
                assert(G.h==data.h);
                const unit_t w  = G.w;
                const unit_t w0 = w-1;
                const unit_t w1 = w-2;
                const unit_t w2 = w-3;

                const unit_t h  = G.h;
                for(unit_t jm=0,j=1,jp=2;j<h;++jm,++j,++jp)
                {
                    pixmap<double>::row           &Gj  = G[j];
                    const typename pixmap<T>::row &dj  = data[j];
                    const typename pixmap<T>::row &djm = data[jm];
                    const typename pixmap<T>::row &djp = data[jp];

                    //__________________________________________________________
                    //
                    // left side
                    //__________________________________________________________
                    {
                        const double gy = double(djp[0])-double(djm[0]);
                        const double gx = 4.0 * double(dj[1]) - 3.0 * double(dj[0]) - double(dj[2]);
                        YOCTO_GFX_GRADIENT_COMPUTE(Gj[0]);
                    }

                    //__________________________________________________________
                    //
                    // right side
                    //__________________________________________________________
                    {
                        const double gy = double(djp[w0])-double(djm[w0]);
                        const double gx = 4.0 * double(dj[w1]) - 3.0 * double(dj[w0]) - double(dj[w2]);
                        YOCTO_GFX_GRADIENT_COMPUTE(Gj[w0]);
                    }
                }

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

                    //__________________________________________________________
                    //
                    // down
                    //__________________________________________________________
                    {
                        const double gx = double(d0[ip]) - double(d0[im]);
                        const double gy = 4.0*double(data[0][i]) - 3.0 * double(data[1][i]) - double(data[2][i]);
                        YOCTO_GFX_GRADIENT_COMPUTE(G0[i]);
                    }

                    //__________________________________________________________
                    //
                    // top
                    //__________________________________________________________
                    {
                        const double gx = double(dh0[ip]) - double(dh0[im]);
                        const double gy = 4.0*double(dh0[i]) - 3.0 * double(dh1[i]) - double(dh2[i]);
                        YOCTO_GFX_GRADIENT_COMPUTE(Gh0[i]);
                    }
                }

                //______________________________________________________________
                //
                // and corners...
                //______________________________________________________________
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

            template <typename T> inline
            double compute1(ipatches          &input,
                            pixmap<double>    &G,
                            const pixmap<T>   &data,
                            threading::server &psrv)
            {
                start(input, G, data, psrv);
                double Gmax = borders(G,data);
                psrv.flush();
                for(size_t i=0;i<input.size;++i)
                {
                    const double tmp = input[i].Gmax;
                    if(tmp>Gmax) Gmax = tmp;
                }
                return Gmax;
            }

            template <typename T> inline
            double compute1(pixmap<double>    &G,
                            const pixmap<T>   &data)
            {
                // prepare inside patch
                coord2D offset;
                coord2D length;
                patch::setup_parallel_metrics(offset, length, G.w, G.h, false);
                const patch2D p(offset,parallel::__coord_dec(offset+length));
                ipatch        in(p);
                in.target   = &G;
                in.source   = &data;

                // initialize with borders
                double Gmax = borders(G,data);
                faked_lock  access;

                // compute inside
                in.inside<T>(access);
                if(in.Gmax>Gmax)
                {
                    Gmax = in.Gmax;
                }

                // done
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
        
        
    }
    
}

#endif
