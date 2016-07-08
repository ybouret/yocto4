#ifndef YOCTO_GRAPHICS_OPS_DIFFERENTIAL_INCLUDED
#define YOCTO_GRAPHICS_OPS_DIFFERENTIAL_INCLUDED 1

#include "yocto/graphics/xpatch.hpp"
#include "yocto/graphics/rawpix.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace graphics
    {

        enum differential_type
        {
            use_gradient,
            use_laplacian
        };

        class differential : public pixmap<real_t>
        {
        public:
            typedef real_t (*proc3)(const real_t,const real_t,const real_t);
            typedef real_t (*proc2)(const real_t,const real_t);

            struct io_data
            {
                real_t vmin;
                real_t vmax;
                proc3  eval_core;
                proc3  eval_side;
                proc2  func;
            };



            explicit differential(const unit_t W, const unit_t H);
            virtual ~differential() throw();

            template <typename T,typename U>
            inline void compute(pixmap<U>               &tgt,
                                const pixmap<T>         &src,
                                const differential_type &ops,
                                xpatches                &xps,
                                threading::engine       *server )
            {
                // first pass: evaluate raw gradient
                target = &tgt;
                source = &src;
                assert(xps.size()>=1);
                const size_t np = xps.size();

                proc3 _core = 0;
                proc3 _side = 0;
                proc2 _func = 0;
                switch(ops)
                {
                    case use_gradient:
                        _core = G_core;
                        _side = G_side;
                        _func = G_func;
                        break;

                    case use_laplacian:
                        _core = L_core;
                        _side = L_side;
                        _func = L_func;
                        break;
                }


                for(size_t i=np;i>0;--i)
                {
                    xpatch &xp    = xps[i];
                    io_data &data = xp.make<io_data>();
                    data.vmin = 0;
                    data.vmax = 0;
                    data.eval_core = _core;
                    data.eval_side = _side;
                    data.func      = _func;
                    xp.enqueue(this, & differential::evaluate<T>,server);
                }
                if(server) server->flush();

                // reduce amplitude
                real_t vmin = xps[1].as<io_data>().vmin;
                real_t vmax = xps[1].as<io_data>().vmax;
                for(size_t i=np;i>1;--i)
                {
                    const io_data &data = xps[i].as<io_data>();
                    vmin = min_of(vmin,data.vmin);
                    vmax = max_of(vmax,data.vmax);
                }
                std::cerr << "vmin=" << vmin <<", vmax=" << vmax << std::endl;

                // normalize
                for(size_t i=np;i>0;--i)
                {
                    xpatch &xp    = xps[i];
                    io_data &data = xp.make<io_data>();
                    data.vmin     = vmin;
                    data.vmax     = vmax;
                    switch(ops)
                    {
                        case use_gradient:
                            xp.enqueue(this, & differential::toGrad<U>,server);
                            break;

                        case use_laplacian:
                            xp.enqueue(this, & differential::toEdge<U>,server);
                            break;
                    }

                }
                if(server) server->flush();
            }



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(differential);
            void       *target;
            const void *source;
            static real_t  G_core(const real_t um,
                                  const real_t u0,
                                  const real_t u1) throw();

            static real_t  G_side(const real_t u0,
                                  const real_t u1,
                                  const real_t u2) throw();

            static real_t  G_func(const real_t dx,
                                  const real_t dy) throw();

            static real_t  L_core(const real_t um,
                                  const real_t u0,
                                  const real_t u1) throw();

            static real_t  L_side(const real_t u0,
                                  const real_t u1,
                                  const real_t u2) throw();

            static real_t  L_func(const real_t dx,
                                  const real_t dy) throw();

            template <typename T>
            inline void evaluate( xpatch &xp, lockable & ) throw()
            {
                assert(source);
                const pixmap<T> &src        = *static_cast<const pixmap<T> *>(source);
                io_data         &data       = xp.as<io_data>();
                real_t          &vmin       = data.vmin;
                real_t          &vmax       = data.vmax;
                const unit_t     xmin       = xp.lower.x;
                const unit_t     xmax       = xp.upper.x;
                const unit_t     ymin       = xp.lower.y;
                const unit_t     ymax       = xp.upper.y;
                proc3            eval_core  = data.eval_core;
                proc3            eval_side  = data.eval_side;
                proc2            func       = data.func;
                bool             init       = true;
                pixmap<real_t>  &self       = *this;

                for(unit_t j=ymax;j>=ymin;--j)
                {
                    const position_type jpos = y_position(j);
                    for(unit_t i=xmax;i>=xmin;--i)
                    {
                        const real_t src0 = src[j][i];

                        // Get the dy component
                        real_t         dy = 0;
                        switch(jpos)
                        {
                            case at_lower:
                                dy = eval_side(src0,src[j+1][i],src[j+2][i]);
                                break;

                            case is_inner:
                                dy = eval_core(src[j-1][i],src0,src[j+1][i]);
                                break;

                            case at_upper:
                                dy = -eval_side(src0,src[j-1][i],src[j-2][i]);
                                break;
                        }

                        real_t dx = 0;
                        switch( x_position(i) )
                        {
                            case at_lower:
                                dx = eval_side(src0,src[j][i+1],src[j][i+2]);
                                break;

                            case is_inner:
                                dx = eval_core(src[j][i-1],src0,src[j][i+1]);
                                break;

                            case at_upper:
                                dx = -eval_side(src0,src[j][i-1],src[j][i-2]);
                                break;
                        }

                        const real_t value = func(dx,dy);
                        self[j][i] = value;
                        if(init)
                        {
                            init = false;
                            vmin = vmax = value;
                        }
                        else
                        {
                            vmin = min_of(vmin,value);
                            vmax = max_of(vmax,value);
                        }
                    }
                }
            }

            template <typename U>
            inline void toGrad( xpatch &xp, lockable & ) throw()
            {
                assert(target);
                pixmap<U>             &tgt  = *static_cast< pixmap<U> *>(target);
                io_data               &data = xp.as<io_data>();
                const real_t           vmin = data.vmin;
                const real_t           vmax = data.vmax;
                const unit_t           xmin = xp.lower.x;
                const unit_t           xmax = xp.upper.x;
                const unit_t           ymin = xp.lower.y;
                const unit_t           ymax = xp.upper.y;
                const pixmap<real_t>  &self = *this;
                assert(vmax>=vmin);
                if(vmax<=vmin)
                {
                    for(unit_t j=ymax;j>=ymin;--j)
                    {
                        for(unit_t i=xmax;i>=xmin;--i)
                        {
                            tgt[j][i] = 0;
                        }
                    }
                }
                else
                {
                    const real_t fac = 1.0f/vmax;
                    for(unit_t j=ymax;j>=ymin;--j)
                    {
                        for(unit_t i=xmax;i>=xmin;--i)
                        {
                            const real_t v = clamp<real_t>(0,self[j][i]*fac,1);
                            tgt[j][i] = gist::float_to<U>(v);
                        }
                    }
                }
            }

            template <typename U>
            inline void toEdge( xpatch &xp, lockable & ) throw()
            {
                assert(target);
                pixmap<U>             &tgt  = *static_cast< pixmap<U> *>(target);
                io_data               &data = xp.as<io_data>();
                const real_t           vmin = data.vmin;
                const real_t           vmax = data.vmax;
                const unit_t           xmin = xp.lower.x;
                const unit_t           xmax = xp.upper.x;
                const unit_t           ymin = xp.lower.y;
                const unit_t           ymax = xp.upper.y;
                const pixmap<real_t>  &self = *this;
                assert(vmax>=vmin);
                if(vmax<=vmin)
                {
                    for(unit_t j=ymax;j>=ymin;--j)
                    {
                        for(unit_t i=xmax;i>=xmin;--i)
                        {
                            tgt[j][i] = 0;
                        }
                    }
                }
                else
                {
                    for(unit_t j=ymax;j>=ymin;--j)
                    {
                        for(unit_t i=xmax;i>=xmin;--i)
                        {
                            const real_t u = self[j][i];
                            if(u<0)
                            {
                                tgt[j][i] = gist::float_to<U>((u/vmin) );
                            }
                            else
                            {
                                if(u>0)
                                {
                                    tgt[j][i] = gist::float_to<U>(u/vmax);
                                }
                                else
                                {
                                    tgt[j][i] = 0;
                                }
                            }
                        }
                    }
                }
            }



            
            
        };
        
    }
    
}

#endif