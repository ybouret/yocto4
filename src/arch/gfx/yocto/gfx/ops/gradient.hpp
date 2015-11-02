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
const real_type gg = math::Hypotenuse(gx, gy);   \
VALUE           = gg;                            \
if(gg>Gmax) { Gmax = gg; }                       \
} while(false)

        struct gradient
        {

            typedef float              real_type;
            typedef pixmap<real_type>  pixmap_t;

            //! inner patch
            class ipatch : public patch
            {
            public:
                real_type       Gmax;
                pixmap_t       *target;
                const void     *source;

                explicit ipatch( const patch2D &p ) throw();
                virtual ~ipatch() throw();

                template <typename T>
                inline void inside(lockable &) throw()
                {
                    assert(source);
                    assert(target);
                    const pixmap<T> &data = *(const pixmap<T> *)(source);
                    pixmap_t        &G    = *target;
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
                        pixmap_t::row                 &Gj  = G[j];
                        const typename pixmap<T>::row &dj  = data[j];
                        const typename pixmap<T>::row &djm = data[jm];
                        const typename pixmap<T>::row &djp = data[jp];

                        for(unit_t im=xlom,i=xlo,ip=xlop;i<xhi;++im,++i,++ip)
                        {
                            const real_type gx = real_type(dj[ip])-real_type(dj[im]);
                            const real_type gy = real_type(djp[i])-real_type(djm[i]);
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
                real_type                Gmax;    //!< for normalisation
                void                 *target;  //!< pixmap<T>
                const pixmap_t       *source;  //!< precomputed

                template <typename T>
                inline void finalize( lockable & ) throw()
                {
                    assert(source);
                    assert(target);
                    pixmap<T>             &grad = *(pixmap<T> *)(target);
                    const pixmap_t        &G    = *source;
                    assert(G.w==grad.w);
                    assert(G.h==grad.h);
                    assert(Gmax>0);
                    const unit_t     xlo  = area.x;
                    const unit_t     xhi  = area.xout;
                    const unit_t     ylo  = area.y;
                    const unit_t     yhi  = area.yout;
                    for(unit_t j=ylo;j<yhi;++j)
                    {
                        const typename pixmap<real_type>::row &Gj = G[j];
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
                              pixmap_t          &G,
                              const pixmap<T>   &data,
                              threading::engine *psrv)
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
                        psrv->enqueue(&p,&ipatch::inside<T>);
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
            static real_type borders(pixmap_t        &G,
                                  const pixmap<T> &data) throw()
            {
                real_type Gmax = 0;
                assert(G.w==data.w);
                assert(G.h==data.h);
                const unit_t w  = G.w;
                const unit_t w0 = w-1;
                const unit_t w1 = w-2;
                const unit_t w2 = w-3;

                const unit_t h  = G.h;
                for(unit_t jm=0,j=1,jp=2;jp<h;++jm,++j,++jp)
                {
                    pixmap_t::row           &Gj  = G[j];
                    const typename pixmap<T>::row &dj  = data[j];
                    const typename pixmap<T>::row &djm = data[jm];
                    const typename pixmap<T>::row &djp = data[jp];

                    //__________________________________________________________
                    //
                    // left side
                    //__________________________________________________________
                    {
                        const real_type gy = real_type(djp[0])-real_type(djm[0]);
                        const real_type gx = real_type(4) * real_type(dj[1]) - real_type(3) * real_type(dj[0]) - real_type(dj[2]);
                        YOCTO_GFX_GRADIENT_COMPUTE(Gj[0]);
                    }

                    //__________________________________________________________
                    //
                    // right side
                    //__________________________________________________________
                    {
                        const real_type gy = real_type(djp[w0])-real_type(djm[w0]);
                        const real_type gx = real_type(4) * real_type(dj[w1]) - real_type(3) * real_type(dj[w0]) - real_type(dj[w2]);
                        YOCTO_GFX_GRADIENT_COMPUTE(Gj[w0]);
                    }
                }

                const typename pixmap<T>::row &d0  = data[0];
                pixmap_t::row                 &G0  = G[0];
                const unit_t h0 = h-1;
                const unit_t h1 = h-2;
                const unit_t h2 = h-3;
                const typename pixmap<T>::row &dh0  = data[h0];
                const typename pixmap<T>::row &dh1  = data[h1];
                const typename pixmap<T>::row &dh2  = data[h2];
                pixmap_t::row                 &Gh0  = G[h0];

                for(unit_t im=0,i=1,ip=2;ip<w;++im,++i,++ip)
                {

                    //__________________________________________________________
                    //
                    // down
                    //__________________________________________________________
                    {
                        const real_type gx = real_type(d0[ip]) - real_type(d0[im]);
                        const real_type gy = real_type(4)*real_type(data[0][i]) - real_type(3) * real_type(data[1][i]) - real_type(data[2][i]);
                        YOCTO_GFX_GRADIENT_COMPUTE(G0[i]);
                    }

                    //__________________________________________________________
                    //
                    // top
                    //__________________________________________________________
                    {
                        const real_type gx = real_type(dh0[ip]) - real_type(dh0[im]);
                        const real_type gy = real_type(4)*real_type(dh0[i]) - real_type(3) * real_type(dh1[i]) - real_type(dh2[i]);
                        YOCTO_GFX_GRADIENT_COMPUTE(Gh0[i]);
                    }
                }

                //______________________________________________________________
                //
                // and corners...
                //______________________________________________________________
                {
                    const real_type gx = real_type(4) * real_type(data[0][0]) - real_type(3) * real_type(data[0][1]) - real_type(data[0][2]);
                    const real_type gy = real_type(4) * real_type(data[0][0]) - real_type(3) * real_type(data[1][0]) - real_type(data[2][0]);
                    YOCTO_GFX_GRADIENT_COMPUTE(G[0][0]);
                }

                {
                    const real_type gx = real_type(4) * real_type(data[0][w0]) - real_type(3) * real_type(data[0][w1]) - real_type(data[0][w2]);
                    const real_type gy = real_type(4) * real_type(data[0][w0]) - real_type(3) * real_type(data[1][w0]) - real_type(data[2][w0]);
                    YOCTO_GFX_GRADIENT_COMPUTE(G[0][w0]);
                }

                {
                    const real_type gx = real_type(4) * real_type(data[h0][0]) - real_type(3) * real_type(data[h0][1]) - real_type(data[h0][2]);
                    const real_type gy = real_type(4) * real_type(data[h0][0]) - real_type(3) * real_type(data[h1][0]) - real_type(data[h2][0]);
                    YOCTO_GFX_GRADIENT_COMPUTE(G[h0][0]);
                }

                {
                    const real_type gx = real_type(4) * real_type(data[h0][w0]) - real_type(3) * real_type(data[h0][w1]) - real_type(data[h0][w2]);
                    const real_type gy = real_type(4) * real_type(data[h0][w0]) - real_type(3) * real_type(data[h1][w0]) - real_type(data[h2][w0]);
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
            static real_type compute1(ipatches           &input,
                                   pixmap<real_type>     &G,
                                   const pixmap<T>    &data,
                                   threading::engine  *psrv)
            {
                start(input, G, data, psrv);
                real_type Gmax = borders(G,data);
                if(psrv)
                {
                    psrv->flush();
                }

                const size_t n = input.size;
                for(size_t i=0;i<n;++i)
                {
                    const real_type tmp = input[i].Gmax;
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
                                 const pixmap<real_type> &G,
                                 const real_type          Gmax,
                                 threading::engine    *psrv)
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
                            psrv->enqueue(&p,&opatch::finalize<T>);
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
                                threading::engine *psrv)
            {
                assert(grad.w==data.w);
                assert(grad.h==data.h);
                // prepare patches
                ipatches     igrad;
                opatches     ograd;
                const size_t cpus = (psrv?psrv->size:1);
                setup(igrad,ograd,cpus,data);

                // effective gradient allocation
                pixmap_t G(data.w,data.h);

                // two passes gradient
                const real_type Gmax =
                compute1(igrad, G, data, psrv);
                compute2(ograd,grad, G, Gmax,psrv);
            }


        };


    }
    
}

#endif
