#ifndef YOCTO_GFX_OPS_DIFFERENTIAL_INCLUDED
#define YOCTO_GFX_OPS_DIFFERENTIAL_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/gfx/xpatch.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/container/tuple.hpp"
#include "yocto/gfx/pixel.hpp"

namespace yocto
{
    namespace gfx
    {
        class differential
        {
        public:
            enum op_type
            {
                gradient,
                laplacian
            };


            YOCTO_PAIR_DECL(info, real_t, vmin, real_t, vmax);
            YOCTO_PAIR_END();


            typedef real_t (*core_proc)(real_t Um, real_t U0, real_t Up);
            typedef real_t (*side_proc)(real_t U0, real_t U1, real_t U2);
            typedef real_t (*eval_proc)(real_t vx, real_t vy);

            explicit differential();
            virtual ~differential() throw();

            template <typename COLOR, typename T, size_t NCH>
            void apply(pixmap<COLOR>         &target,
                       const pixmap<COLOR>   &source,
                       pixmaps<real_t>       &channels,
                       const op_type          ops,
                       xpatches              &xps,
                       threading::engine     *server)
            {
                tgt      = &target;
                src      = &source;
                chn      = &channels;

                switch(ops)
                {
                    case gradient:
                        coreProc   = core_proc_g;
                        sideProcLo = side_proc_g_lo;
                        sideProcUp = side_proc_g_up;
                        evalProc   = eval_proc_g;
                        break;

                    case laplacian:
                        coreProc   = core_proc_l;
                        sideProcLo = side_proc_l;
                        sideProcUp = side_proc_l;
                        evalProc   = eval_proc_l;
                        break;
                }

                const size_t np = xps.size();
                for(size_t i=np;i>0;--i)
                {
                    xpatch &xp = xps[i];
                    xp.build<info,real_t,real_t>(0,0);
                    xp.enqueue(this,&differential::initialize<COLOR,T,NCH>,server);
                }
                if(server) server->flush();

                global = xps[1].as<info>();
                for(size_t i=np;i>1;--i)
                {
                    xpatch     &xp  = xps[i];
                    const info &tmp = xp.as<info>();
                    global.vmin = min_of(global.vmin,tmp.vmin);
                    global.vmax = max_of(global.vmax,tmp.vmax);
                }

                if(global.vmax<=global.vmin)
                {
                    target.ldz();
                }
                else
                {
                    for(size_t i=np;i>0;--i)
                    {
                        xpatch     &xp  = xps[i];
                        switch(ops)
                        {
                            case gradient:
                                xp.enqueue(this,&differential::update_g<COLOR,T,NCH>,server);
                                break;

                            case laplacian:
                                xp.enqueue(this,&differential::update_l<COLOR,T,NCH>,server);
                                break;
                        }
                    }
                    if(server) server->flush();
                }

            }

            static real_t core_proc_g(real_t Um, real_t U0, real_t Up) throw();
            static real_t side_proc_g_lo(real_t U0, real_t U1, real_t U2) throw();
            static real_t side_proc_g_up(real_t U0, real_t U1, real_t U2) throw();
            static real_t eval_proc_g(real_t gx, real_t gy) throw();

            static real_t core_proc_l(real_t Um, real_t U0, real_t Up) throw();
            static real_t side_proc_l(real_t U0, real_t U1, real_t U2) throw();
            static real_t eval_proc_l(real_t gx, real_t gy) throw();


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(differential);
            void            *tgt;
            const void      *src;
            pixmaps<real_t> *chn;
            core_proc        coreProc;
            side_proc        sideProcLo;
            side_proc        sideProcUp;
            eval_proc        evalProc;
        public:
            info             global;

        private:
            template <typename COLOR,typename T,size_t NCH>
            void initialize( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);
                assert(chn);
                assert(coreProc);
                assert(sideProcLo);
                assert(sideProcUp);
                assert(evalProc);

                bool                 init     = true;
                pixmaps<real_t>     &channels = *chn;
                const pixmap<COLOR> &source   = *static_cast< const pixmap<COLOR> *>(src);
                const unit_t         ymin     = xp.lower.y;
                const unit_t         ymax     = xp.upper.y;
                const unit_t         xmin     = xp.lower.x;
                const unit_t         xmax     = xp.upper.x;

                info   &params = xp.as<info>();
                real_t &vmin   = params.vmin;
                real_t &vmax   = params.vmax;

                for(unit_t y=ymax;y>=ymin;--y)
                {
                    const bitmap::position_type ypos = source.y_position(y);
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        const  COLOR &C0 = source[y][x];
                        const  T     *p0 = (const T*)&C0;
                        real_t y_value[NCH];

                        switch(ypos)
                        {
                            case bitmap::at_lower: {
                                assert(0==y);
                                const COLOR &C1 = source[1][x];
                                const COLOR &C2 = source[2][x];
                                const T     *p1 = (const T*)&C1;
                                const T     *p2 = (const T*)&C2;
                                for(size_t k=0;k<NCH;++k)
                                {
                                    y_value[k] = sideProcLo(p0[k],p1[k],p2[k]);
                                }
                            } break;

                            case bitmap::is_inner: {
                                const COLOR &Cm = source[y-1][x];
                                const COLOR &Cp = source[y+1][x];
                                const T     *pm = (const T*)&Cm;
                                const T     *pp = (const T*)&Cp;
                                for(size_t k=0;k<NCH;++k)
                                {
                                    y_value[k] = coreProc(pm[k],p0[k],pp[k]);
                                }
                            } break;


                            case bitmap::at_upper: {
                                assert(source.h-1==y);
                                const COLOR &C1 = source[y-1][x];
                                const COLOR &C2 = source[y-2][x];
                                const T     *p1 = (const T*)&C1;
                                const T     *p2 = (const T*)&C2;
                                for(size_t k=0;k<NCH;++k)
                                {
                                    y_value[k] = sideProcUp(p0[k],p1[k],p2[k]);
                                }
                            } break;
                        }

                        real_t x_value[NCH];
                        switch( source.x_position(x) )
                        {
                            case bitmap::at_lower: {
                                assert(0==x);
                                const COLOR &C1 = source[y][1];
                                const COLOR &C2 = source[y][2];
                                const T     *p1 = (const T*)&C1;
                                const T     *p2 = (const T*)&C2;
                                for(size_t k=0;k<NCH;++k)
                                {
                                    x_value[k] = sideProcLo(p0[k],p1[k],p2[k]);
                                }

                            } break;

                            case bitmap::is_inner: {
                                const COLOR &Cm = source[y][x-1];
                                const COLOR &Cp = source[y][x+1];
                                const T     *pm = (const T*)&Cm;
                                const T     *pp = (const T*)&Cp;
                                for(size_t k=0;k<NCH;++k)
                                {
                                    x_value[k] = coreProc(pm[k],p0[k],pp[k]);
                                }
                            } break;

                            case bitmap::at_upper: {
                                assert(source.w-1==x);
                                const COLOR &C1 = source[y][x-1];
                                const COLOR &C2 = source[y][x-2];
                                const T     *p1 = (const T*)&C1;
                                const T     *p2 = (const T*)&C2;
                                for(size_t k=0;k<NCH;++k)
                                {
                                    x_value[k] = sideProcUp(p0[k],p1[k],p2[k]);
                                }

                            } break;
                        }

                        if(init)
                        {
                            const real_t tmp = evalProc(x_value[0],y_value[0]);
                            vmin = vmax = tmp;
                            channels[0][y][x] = tmp;
                            init=false;
                        }
                        else
                        {
                            const real_t tmp = evalProc(x_value[0],y_value[0]);
                            vmin = min_of(vmin,tmp);
                            vmax = max_of(vmax,tmp);
                            channels[0][y][x] = tmp;
                        }

                        for(size_t k=1;k<NCH;++k)
                        {
                            const real_t tmp = evalProc(x_value[k],y_value[k]);
                            channels[k][y][x] = tmp;
                            vmin = min_of(vmin,tmp);
                            vmax = max_of(vmax,tmp);
                        }

                    }
                }
            }

            template <typename COLOR,typename T,size_t NCH>
            void update_g( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);
                assert(chn);

                pixmaps<real_t>     &channels = *chn;
                pixmap<COLOR>       &target   = *static_cast< pixmap<COLOR> *>(tgt);
                const unit_t         ymin     = xp.lower.y;
                const unit_t         ymax     = xp.upper.y;
                const unit_t         xmin     = xp.lower.x;
                const unit_t         xmax     = xp.upper.x;
                const real_t         scale    = real_t(pixel<T>::opaque);
                const real_t         vmin     = global.vmin;
                const real_t         delta    = global.vmax-vmin;
                for(unit_t y=ymax;y>=ymin;--y)
                {
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        COLOR &C = target[y][x];
                        T     *q = (T *)&C;
                        for(size_t k=0;k<NCH;++k)
                        {
                            const real_t value = channels[k][y][x];
                            q[k] = static_cast<T>( (scale*(value-vmin))/delta );
                        }
                    }
                }
            }

            template <typename COLOR,typename T,size_t NCH>
            void update_l( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);
                assert(chn);

                pixmaps<real_t>     &channels = *chn;
                pixmap<COLOR>       &target   = *static_cast< pixmap<COLOR> *>(tgt);
                const unit_t         ymin     = xp.lower.y;
                const unit_t         ymax     = xp.upper.y;
                const unit_t         xmin     = xp.lower.x;
                const unit_t         xmax     = xp.upper.x;
                const real_t         scale    = real_t(pixel<T>::opaque);
                const real_t         delta    = max_of(-global.vmin,global.vmax);
                for(unit_t y=ymax;y>=ymin;--y)
                {
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        COLOR &C = target[y][x];
                        T     *q = (T *)&C;
                        for(size_t k=0;k<NCH;++k)
                        {
                            const real_t value = channels[k][y][x];
                            q[k] = static_cast<T>( (scale*math::Fabs(value))/delta );
                        }
                    }
                }
            }


            
        };
    }
}

#endif
