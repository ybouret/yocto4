#ifndef YOCTO_STENCIL_OPS_INCLUDED
#define YOCTO_STENCIL_OPS_INCLUDED 1

#include "yocto/graphics/pixmaps.hpp"
#include "yocto/graphics/parallel.hpp"

namespace yocto
{
    namespace graphics
    {
        
        
        
        //! a 3x3 stencil
        struct stencil
        {
        public:
            typedef float     real_type;
            
            //__________________________________________________________________
            //
            // handling 3x3 values
            //__________________________________________________________________
            class weights
            {
            public:
                typedef float     real_type;
                real_type         value[3][3];
                
                inline real_type & operator()(unit_t dx,unit_t dy) throw()
                {
                    assert(dx>=-1); assert(dx<=1);
                    assert(dy>=-1); assert(dy<=1);
                    return value[++dx][++dy];
                }
                
                inline const real_type & operator()(unit_t dx,unit_t dy) const throw()
                {
                    assert(dx>=-1); assert(dx<=1);
                    assert(dy>=-1); assert(dy<=1);
                    return value[++dx][++dy];
                }
                
                weights() throw();
                ~weights() throw();
                weights(const weights &) throw();
                weights & operator=(const weights&) throw();
                void ldz() throw();
                inline friend std::ostream & operator<<(std::ostream &os, const weights &w)
                {
                    os << "[ " << w.value[0][0] << " " << w.value[0][1] << " " << w.value[0][2] << "; ";
                    os << w.value[1][0] << " " << w.value[1][1] << " " << w.value[1][2] << "; ";
                    os << w.value[2][0] << " " << w.value[2][1] << " " << w.value[2][2] << "]";
                    return os;
                }
            };
            
            //__________________________________________________________________
            //
            // a patch
            //__________________________________________________________________
            class patch : public graphics::patch
            {
            public:
                pixmap<real_type> *target;
                void              *handle; //!< pixmap
                gist::filling      f; //!< filling modes
                const weights     *w; //!< user's weights
                real_type          vmin;
                real_type          vmax;
                weights            s; //!< local stencil
                
                explicit patch(const graphics::patch &p) throw();
                patch(const patch &p) throw();
                virtual ~patch() throw();
                
                template <typename T>
                void compute(lockable &) throw()
                {
                    assert(handle);
                    assert(target);
                    assert(w);
                    
                    const pixmap<T>   &pxm = *static_cast<pixmap<T> *>(handle);
                    pixmap<real_type> &tgt = *target;
                    const real_type   *lhs = &(w->value[0][0]);
                    const real_type   *rhs = &(s.value[0][0]);
                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            stencil::load<T>(s,pxm,i,j,f);
                            real_type sum = 0;
                            for(size_t k=0;k<9;++k)
                            {
                                sum += lhs[k] * rhs[k];
                            }
                            tgt[j][i] = sum;
                        }
                    }
                    vmin = vmax = tgt[lower.y][lower.x];
                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            const real_type tmp = tgt[j][i];
                            if(tmp<vmin) vmin = tmp;
                            if(tmp>vmax) vmax = tmp;
                        }
                    }
                }
                
                template <typename T>
                void transfer(lockable &) throw()
                {
                    assert(handle);
                    assert(target);
                    const double             den = vmax-vmin; assert(den>0);
                    pixmap<T>               &pxm = *static_cast<pixmap<T> *>(handle);
                    const pixmap<real_type> &src = *target;
                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            const real_type p = (src[j][i]-vmin)/den;
                            pxm[j][i] = p;
                        }
                    }
                }
                
                
            private:
                YOCTO_DISABLE_ASSIGN(patch);
            };
            
            typedef vector<patch> patches;
            
            static void create(patches &sp, const graphics::patch &surface, threading::engine *server);
            
            
            static const int atLeft  = 0x01;
            static const int atRight = 0x02;
            static const int atBot   = 0x04;
            static const int atTop   = 0x08;
            
            template <typename T> static inline
            void load(weights            &s,
                      const  pixmap<T>   &p,
                      const  unit_t       i,
                      const  unit_t       j,
                      const gist::filling f) throw()
            {
                assert(i>=0);
                assert(i<p.w);
                assert(j>=0);
                assert(j<p.h);
                
                const unit_t w0    = p.upper.x;
                const unit_t h0    = p.upper.y;
                int          flags = 0;
                if(i<=0) { flags |= atLeft; } else{ if(i>=w0) {flags |= atRight;} }
                if(j<=0) { flags |= atBot;  } else{ if(j>=h0) {flags |= atTop;}   }
                
                
                s.ldz(); //!< todo: remove
                switch(flags)
                {
                    case 0:
                        for(unit_t dy=-1;dy<=1;++dy)
                        {
                            for(unit_t dx=-1;dx<=1;++dx)
                            {
                                s(dx,dy) = real_type(p[j+dy][i+dx]);
                            }
                        }
                        break;
                        
                    case atLeft:
                        for(unit_t dy=-1;dy<=1;++dy)
                        {
                            for(unit_t dx=0;dx<=1;++dx)
                            {
                                s(dx,dy) = real_type(p[j+dy][i+dx]);
                            }
                            s(-1,dy) = gist::get_fill<real_type,T>(p[j][i], p[j][i+1],p[j][i+2],f);
                        }
                        break;
                        
                    case atRight:
                        for(unit_t dy=-1;dy<=1;++dy)
                        {
                            for(unit_t dx=-1;dx<=0;++dx)
                            {
                                s(dx,dy) = real_type(p[j+dy][i+dx]);
                            }
                            s(1,dy) = gist::get_fill<real_type,T>(p[j][i], p[j][i-1],p[j][i-2],f);
                        }
                        break;
                        
                    case atBot:
                        for(unit_t dx=-1;dx<=1;++dx)
                        {
                            const unit_t ii = i+dx;
                            for(unit_t dy=0;dy<=1;++dy)
                            {
                                s(dx,dy) = real_type(p[j+dy][ii]);
                            }
                            s(dx,-1) = gist::get_fill<real_type,T>(p[j][ii], p[j+1][ii], p[j+2][ii],f);
                        }
                        break;
                        
                    case atTop:
                        for(unit_t dx=-1;dx<=1;++dx)
                        {
                            const unit_t ii = i+dx;
                            for(unit_t dy=-1;dy<=0;++dy)
                            {
                                s(dx,dy) = real_type(p[j+dy][ii]);
                            }
                            s(dx,1) = gist::get_fill<real_type,T>(p[j][ii], p[j-1][ii], p[j-2][ii],f);
                        }
                        break;
                        
                    case atBot|atLeft:
                        
                        break;
                        
                    case atBot|atRight:
                        
                        break;
                        
                    case atTop|atLeft:
                        
                        break;
                        
                    case atTop|atRight:
                        
                        break;

                        
                    default:
                        assert(die("never get here"));
                }
            }
            
            //! compute the field from the pixmap
            template <typename T>
            static inline
            void launch(patches            &sp,
                        pixmap<real_type>  &field,
                        const weights      &w,
                        const pixmap<T>    &pxm,
                        const gist::filling f,
                        threading::engine  *server)
            {
                faked_lock sequential;
                for(size_t i=sp.size();i>0;--i)
                {
                    patch &sub = sp[i];
                    sub.w      = &w;
                    sub.target = &field;
                    sub.handle = (void*)&pxm;
                    sub.f      = f;
                    if(server)
                    {
                        server->enqueue(&sub, &patch::compute<T>);
                    }
                    else
                    {
                        sub.compute<T>(sequential);
                    }
                }
            }
            
            //! compute the pixmap from the field
            template <typename T>
            static inline
            void finish(patches                 &sp,
                        pixmap<real_type>       &field,
                        pixmap<T>               &pxm,
                        threading::engine       *server)
            {
                faked_lock sequential;
                if(server) server->flush();
                real_type vmin = sp[1].vmin;
                real_type vmax = sp[1].vmax;
                for(size_t i=sp.size();i>1;--i)
                {
                    const patch &sub = sp[i];
                    vmin = min_of(vmin,sub.vmin);
                    vmax = max_of(vmax,sub.vmax);
                }
                std::cerr << "vmin=" << vmin << std::endl;
                std::cerr << "vmax=" << vmax << std::endl;
                if(vmax<=vmin)
                {
                    pxm.ldz();
                }
                else
                {
                    for(size_t i=sp.size();i>0;--i)
                    {
                        patch &sub = sp[i];
                        sub.vmin   = vmin;
                        sub.vmax   = vmax;
                        sub.target = &field;
                        sub.handle = (void*)&pxm;
                        if(server)
                        {
                            server->enqueue(&sub, &patch::transfer<T>);
                        }
                        else
                        {
                            sub.transfer<T>(sequential);
                        }
                    }
                    server->flush();
                }
            }
            
        };
    }
}

#endif
