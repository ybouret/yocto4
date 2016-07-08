#ifndef YOCTO_GRAPHICS_OPS_DIFFERENTIAL_INCLUDED
#define YOCTO_GRAPHICS_OPS_DIFFERENTIAL_INCLUDED 1

#include "yocto/graphics/xpatch.hpp"
#include "yocto/graphics/pixmap.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace graphics
    {


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

            template <typename T>
            inline void compute(const pixmap<T>   &src,
                                xpatches           &xps,
                                threading::engine *server )
            {
                source = &src;
                const size_t np = xps.size();
                for(size_t i=np;i>0;--i)
                {
                    xpatch &xp    = xps[i];
                    io_data &data = xp.make<io_data>();
                    data.vmin = 0;
                    data.vmax = 0;
                    data.eval_core = G_core;
                    data.eval_side = G_side;
                    data.func      = G_func;
                    xp.enqueue(this, & differential::evaluate<T>,server);
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

            template <typename T>
            inline void evaluate( xpatch &xp, lockable & ) throw()
            {
                assert(source);
                const pixmap<T> &src   = *static_cast<const pixmap<T> *>(source);
                io_data         &data  = xp.as<io_data>();
                real_t          &vmin  = data.vmin;
                real_t          &vmax  = data.vmax;
                const unit_t     xmin  = xp.lower.x;
                const unit_t     xmax  = xp.upper.x;
                const unit_t     ymin  = xp.lower.y;
                const unit_t     ymax  = xp.upper.y;
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
                        real_t              dy = 0;
                        switch(jpos)
                        {
                            case at_lower:
                                break;

                            case is_inner:
                                dy = eval_core(src[j-1][i],src0,src[j+1][i]);
                                break;

                            case at_upper:
                                break;
                        }

                        real_t dx = 0;
                        if(i<=0)
                        {

                        }
                        else
                        {
                            if(i>=ymax)
                            {

                            }
                            else
                            {
                                dx = eval_core(src[j][i-1],src0,src[j][i+1]);
                            }
                        }

                        const real_t value = func(dx,dy);
                        self[j][i] = value;
                    }
                }
            }
            
        };
        
    }
    
}

#endif