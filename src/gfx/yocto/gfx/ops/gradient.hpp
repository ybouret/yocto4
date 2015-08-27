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

            //! inner patch
            class ipatch : public patch
            {
            public:
                double          Gmax;
                pixmap<double> *target;
                const void     *source;

                explicit ipatch( const patch2D &p ) throw();
                virtual ~ipatch() throw();

                template <typename T>
                inline void inside(lockable &) throw()
                {
                    assert(source);
                    assert(target);
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

                    Gmax = 0;
                    for(unit_t jm=ylo-1,j=ylo,jp=ylo+1;j<yhi;++jm,++j,++jp)
                    {
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

            //! inner patches
            typedef dynamic_slots<ipatch> ipatches;

            //! output patch
            class opatch : public patch
            {
            public:
                explicit opatch(const patch2D &p) throw();
                virtual ~opatch() throw();
                double                Gmax;    //!< for normalisation
                void                 *target;  //!< pixmap<T>
                const pixmap<double> *source;  //!< precomputed

                template <typename T>
                inline void finalize( lockable & ) throw()
                {
                    assert(source);
                    assert(target);
                    pixmap<T>             &grad = *(pixmap<T> *)(target);
                    const pixmap<double>  &G    = *source;
                    assert(G.w==grad.w);
                    assert(G.h==grad.h);
                    assert(Gmax>0);
                    const unit_t     xlo  = area.x;
                    const unit_t     xhi  = area.xout;
                    const unit_t     ylo  = area.y;
                    const unit_t     yhi  = area.yout;
                    for(unit_t j=ylo;j<yhi;++j)
                    {
                        const typename pixmap<double>::row &Gj = G[j];
                        typename pixmap<T>::row            &gj = grad[j];
                        for(unit_t i=xlo;i<xhi;++i)
                        {
                            gj[i] = to_unit<T>(Gj[i]/Gmax);
                        }
                    }
                }

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(opatch);
            };

            //! output patches
            typedef dynamic_slots<opatch> opatches;


            //! setup patches for at most #cpus
            inline
            static void setup(ipatches    &input,
                              opatches    &output,
                              const size_t cpus,
                              const bitmap &bmp)
            {
                input.free();
                patch::setup(input,cpus,bmp,false);

                output.free();
                patch::setup(output,cpus,bmp,true);
            }


            //! start computing
            /**
             store effective gradient in G
             sequential if NULL == psrv
             parallel   otherwise
             \warning server is not flushed...
             */
            template <typename T> inline
            static void start(ipatches          &input,
                              pixmap<double>    &G,
                              const pixmap<T>   &data,
                              threading::server *psrv)
            {
                const size_t n = input.size;
                // compute core
                if(psrv)
                {
                    // parallel
                    for(size_t i=0;i<n;++i)
                    {
                        ipatch &p = input[i];
                        p.target  = &G;
                        p.source  = &data;
                        const threading::server::job J(&p,&ipatch::inside<T>);
                        psrv->enqueue(J);
                    }
                }
                else
                {
                    // sequential
                    faked_lock access;
                    for(size_t i=0;i<n;++i)
                    {
                        ipatch &p = input[i];
                        p.target  = &G;
                        p.source  = &data;
                        p.inside<T>(access);
                    }
                }
            }


            //! borders computing of effective gradient
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
                for(unit_t jm=0,j=1,jp=2;jp<h;++jm,++j,++jp)
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

            //! compute effective gradient everywhere
            /**
             - input patches must be precomputed.
             - server is flushed...
             */
            template <typename T> inline
            static double compute1(ipatches          &input,
                                   pixmap<double>    &G,
                                   const pixmap<T>   &data,
                                   threading::server *psrv)
            {
                start(input, G, data, psrv);
                double Gmax = borders(G,data);
                if(psrv)
                {
                    psrv->flush();
                }

                const size_t n = input.size;
                for(size_t i=0;i<n;++i)
                {
                    const double tmp = input[i].Gmax;
                    if(tmp>Gmax) Gmax = tmp;
                }
                return Gmax;
            }


            //! convert effective gradient to typed gradient
            /**
             - output patches must be precomputed.
             - server is flushed
             */
            template <typename T> inline
            static void compute2(opatches             &output,
                                 pixmap<T>            &grad,
                                 const pixmap<double> &G,
                                 const double          Gmax,
                                 threading::server    *psrv)
            {
                const size_t n = output.size;
                if(Gmax>0)
                {
                    if(psrv)
                    {
                        // parallel
                        for(size_t i=0;i<n;++i)
                        {
                            opatch &p = output[i];
                            p.Gmax    = Gmax;
                            p.target  = &grad;
                            p.source  = &G;
                            const threading::server::job J(&p,&opatch::finalize<T>);
                            psrv->enqueue(J);
                        }
                        psrv->flush();
                    }
                    else
                    {
                        //sequential
                        faked_lock access;
                        for(size_t i=0;i<n;++i)
                        {
                            opatch &p = output[i];
                            p.Gmax    = Gmax;
                            p.target  = &grad;
                            p.source  = &G;
                            p.finalize<T>(access);
                        }
                    }
                }
                else
                {
                    grad.ldz();
                }
            }


            template <typename T> inline
            static void compute(pixmap<T>         &grad,
                                const pixmap<T>   &data,
                                threading::server *psrv)
            {
                assert(grad.w==data.w);
                assert(grad.h==data.h);
                // prepare patches
                ipatches     igrad;
                opatches     ograd;
                const size_t cpus = (psrv?psrv->size:1);
                setup(igrad,ograd,cpus,data);

                // effective gradient allocation
                pixmap<double> G(data.w,data.h);

                // two passes gradient
                const double Gmax =
                compute1(igrad, G, data, psrv);
                compute2(ograd,grad, G, Gmax,psrv);
            }


        };


    }
    
}

#endif
