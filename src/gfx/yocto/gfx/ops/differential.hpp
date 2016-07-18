#ifndef YOCTO_GFX_OPS_DIFFERENTIAL_INCLUDED
#define YOCTO_GFX_OPS_DIFFERENTIAL_INCLUDED 1

#include "yocto/gfx/pixmaps.hpp"
#include "yocto/gfx/xpatch.hpp"

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

            struct info
            {
                real_t vmin,vmax;
            };

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
                coreProc = core_proc_g;
                sideProc = side_proc_g;
                evalProc = eval_proc_g;
                const size_t np = xps.size();
                for(size_t i=np;i>0;--i)
                {
                    xpatch &xp = xps[i];
                    xp.make<info>();
                    xp.enqueue(this,&differential::run<COLOR,T,NCH>,server);
                }
                if(server) server->flush();
            }

            static real_t core_proc_g(real_t Um, real_t U0, real_t Up) throw();
            static real_t side_proc_g(real_t U0, real_t U1, real_t U2) throw();
            static real_t eval_proc_g(real_t gx, real_t gy) throw();


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(differential);
            void            *tgt;
            const void      *src;
            pixmaps<real_t> *chn;
            core_proc   coreProc;
            side_proc   sideProc;
            eval_proc   evalProc;
            
            template <typename COLOR,typename T,size_t NCH>
            void run( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);
                assert(chn);
                assert(coreProc);
                assert(sideProc);
                assert(evalProc);

                bool             init     = true;
                pixmaps<real_t> &channels = *chn;
                const pixmap<T> &source   = *static_cast< const pixmap<COLOR> *>(src);
                const unit_t     ymin     = xp.lower.y;
                const unit_t     ymax     = xp.upper.y;
                const unit_t     xmin     = xp.lower.x;
                const unit_t     xmax     = xp.upper.x;

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
                                    y_value[k] = sideProc(p0[k],p1[k],p2[k]);
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
                                    y_value[k] = -sideProc(p0[k],p1[k],p2[k]);
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
                                    x_value[k] = sideProc(p0[k],p1[k],p2[k]);
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
                                    x_value[k] = -sideProc(p0[k],p1[k],p2[k]);
                                }

                            } break;
                        }

                        if(init)
                        {

                            init=false;
                        }
                        else
                        {

                        }

                    }
                }


            }


        };
    }
}

#endif
