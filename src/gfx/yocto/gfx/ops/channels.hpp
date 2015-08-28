#ifndef YOCTO_GFX_OPS_CHANNELS_INCLUDED
#define YOCTO_GFX_OPS_CHANNELS_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/gfx/parallel.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace gfx
    {


        template <
        typename T = uint8_t,
        typename U = uint8_t,
        template <typename> class COLORSPACE = rgb
        >
        struct channels
        {
            typedef COLORSPACE<U> color;
            static  const size_t  max_count = sizeof(color)/sizeof(U);

            class iopatch : public patch
            {
            public:
                pixmaps<T>    *pch;
                pixmap<color> *ppx;
                size_t         offset; //!<  channel index
                size_t         length; //!< #channels for ops

                inline explicit iopatch(const patch2D &p) throw() :
                patch(p),pch(0),ppx(0),offset(0),length(0) {}
                inline virtual ~iopatch() throw() {}

                void split(lockable&) throw()
                {
                    assert(pch);
                    assert(ppx);
                    const pixmap<color> &px = *ppx;
                    pixmaps<T>          &ch = *pch;
                    const unit_t ylo = area.y;
                    const unit_t yhi = area.yout;
                    const unit_t xlo = area.x;
                    const unit_t xhi = area.xout;
                    const size_t ini = offset;
                    const size_t nch = length;
                    for(unit_t j=ylo;j<yhi;++j)
                    {
                        const typename pixmap<color>::row &src = px[j];
                        for(unit_t i=xlo;i<xhi;++i)
                        {
                            const U *q = (const U *)& src[i];
                            q+=ini;
                            for(size_t c=0;c<nch;++c)
                            {
                                ch[c][j][i] = q[c];
                            }
                        }
                    }
                }

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(iopatch);
            };

            class iopatches : public dynamic_slots<iopatch>
            {
            public:
                inline explicit iopatches(size_t n=0) : dynamic_slots<iopatch>(n) {}
                inline virtual ~iopatches() throw() {}

                inline void setup_for(const size_t cpus, const bitmap &bmp)
                {
                    this->free();
                    patch::setup(*this,cpus,bmp,true);
                }

                inline void split(pixmaps<T>          &ch,
                                  const pixmap<color> &px,
                                  threading::server   *psrv)
                {
                    dynamic_slots<iopatch> &pp = *this;
                    assert(ch.w==px.w);
                    assert(ch.h==px.h);
                    const size_t np     = pp.size;
                    const size_t offset = 0;
                    const size_t length = min_of<size_t>(ch.size,max_count);
                    if(psrv)
                    {
                        for(size_t i=0;i<np;++i)
                        {
                            iopatch &p = pp[i];
                            p.offset   = offset;
                            p.length   = length;
                            p.pch      = &ch;
                            p.ppx      = (pixmap<color> *)&px;
                            const threading::server::job J(&p, & iopatch::split);
                            psrv->enqueue(J);
                        }
                        psrv->flush();
                    }
                    else
                    {
                        faked_lock access;
                        for(size_t i=0;i<np;++i)
                        {
                            iopatch &p = pp[i];
                            p.offset   = offset;
                            p.length   = length;
                            p.pch      = &ch;
                            p.ppx      = (pixmap<color> *)&px;
                            p.split(access);
                        }
                    }

                }

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(iopatches);
            };

            
            
        };

    }

}

#endif

