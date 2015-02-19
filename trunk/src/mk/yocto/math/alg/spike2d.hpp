#ifndef YOCTO_MATH_ALG_SPIKE2D_INCLUDED
#define YOCTO_MATH_ALG_SPIKE2D_INCLUDED 1

#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/container/sequence.hpp"
#include "yocto/core/list.hpp"
#include "yocto/sort/quick.hpp"

#include <iostream>

namespace yocto
{
    namespace math
    {
        template <typename COORD>
        class coordinate
        {
        public:
            coordinate *next;
            coordinate *prev;
            COORD       u,v;

            inline  coordinate() throw() : next(0), prev(0), u(0), v(0)  {}
            inline ~coordinate() throw() {}
            inline  coordinate(COORD a,COORD b) throw() : next(0), prev(0), u(a), v(b) {}

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(coordinate);
        };


        template <typename T,typename COORD>
        class spike2D : public counted_object
        {
        public:
            typedef coordinate<COORD>  coord_t;

            const coord_t              center;
            core::list_of_cpp<coord_t> coords;

            inline spike2D(const COORD u_center,
                           const COORD v_center) throw():
            center(u_center,v_center),
            coords()
            {
            }

            inline virtual ~spike2D() throw()
            {
            }

            typedef arc_ptr<spike2D> pointer;

            class tableau
            {
            public:
                typedef      size_t word_t;
                const COORD  umin;
                const COORD  umax;
                const size_t nu;
                const COORD  vmin;
                const COORD  vmax;
                const size_t nv;
                const size_t items;
                word_t       count;

                explicit tableau(const COORD          Umin,
                                 const COORD          Umax,
                                 const COORD          Vmin,
                                 const COORD          Vmax) :
                umin(Umin),
                umax(Umax),
                nu(umax+1-umin),
                vmin(Vmin),
                vmax(Vmax),
                nv(vmax+1-vmin),
                items(nv*nu),
                count(0),
                wlen(items),
                flag( memory::kind<memory::global>::acquire_as<word_t>(wlen) )
                {
                    for(size_t i=items;i>0;--i) flag[i] = 0;
                }

                inline ~tableau() throw()
                {
                    memory::kind<memory::global>::release_as<word_t>(flag, wlen);
                }

                word_t &operator()(COORD u, COORD v) throw()
                {
                    assert(u>=umin);
                    assert(u<=umax);
                    assert(v>=vmin);
                    assert(v<=vmax);
                    return flag[ (u-umin)*nv + (v-vmin) ];
                }


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(tableau);
                size_t  wlen;
                word_t *flag;

            };

            template <typename FIELD,typename FUNC>
            static inline void detect(sequence<pointer>   &spikes,
                                      const FIELD         &F,
                                      const COORD          umin,
                                      const COORD          umax,
                                      const COORD          vmin,
                                      const COORD          vmax,
                                      FUNC                &proc
                                      )
            {
                assert(umax>=umin);
                assert(vmax>=vmin);
                spikes.free();

                //! tableau of access
                tableau status(umin,umax,vmin,vmax);
                COORD uc(0),vc(0);
                size_t level = 0;
                while( (level=scan(F, proc, status, uc, vc)) > 0 )
                {
                    std::cerr << "spike @" << uc << ", " << vc << std::endl;
                    spike2D       *peak = new spike2D(uc,vc);
                    const pointer  pptr(peak); // is now safe
                    build(*peak, F, status, proc);
                    spikes.push_back(pptr);
                    break;
                }

            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(spike2D);
            template <typename FIELD,typename FUNC> static inline
            size_t scan(const FIELD &F,
                        FUNC    &proc,
                        tableau &status,
                        COORD   &uc,
                        COORD   &vc )  throw()
            {
                T    value(0);

                // first loop: try to find data
                COORD u = status.umin;
                for(;u<=status.umax;++u)
                {
                    for(COORD v=status.vmin;v<=status.vmax;++v)
                    {
                        if(status(u,v)<=0)
                        {
                            status(u,v) = ++status.count; // not available any more
                            uc = u;                       // starting point
                            vc = v;                       // starting point
                            value = proc(F[u][v]);        // max value so far
                            goto HAS_EXTREMUM;
                        }
                    }
                }
                return 0;

            HAS_EXTREMUM:
                // second loop: max over other available data
                for(;u<=status.umax;++u)
                {
                    for(COORD v=status.vmin;v<=status.vmax;++v)
                    {
                        if(status(u,v)<=0)
                        {
                            const T tmp = proc(F[u][v]);
                            if(tmp>value)
                            {
                                uc = u;
                                vc = u;
                                value = tmp;
                            }
                        }
                    }
                }
                assert(status.count>0);
                status(uc,vc) = status.count;
                return status.count;
            }

            template <typename FIELD, typename FUNC> static inline
            void build( spike2D &peak, const FIELD &F, tableau &status, FUNC &proc )
            {
                const unit_t u0(peak.center.u);
                const unit_t v0(peak.center.v);
                const size_t ident = status(u0,v0); //!< zone identifier
                const T      level = F[u0][v0];     //!< extremum

                std::cerr << "expanding peak #" << ident << " around " << level << " @" << u0 << "," << v0 << std::endl;

                for(unit_t radius=1;;++radius)
                {
                    bool has_link = false;
                    unit_t r = radius;
                    unit_t x = -r, y = 0, err = 2-2*r;
                    do {
                        //setPixel(xm-x, ym+y);
                        //setPixel(xm-y, ym-x);
                        //setPixel(xm+x, ym-y);
                        //setPixel(xm+y, ym+x);
                        has_link = is_connected_to(peak, F, status, ident, level, u0-x, v0+y, proc);
                        has_link = is_connected_to(peak, F, status, ident, level, u0-y, v0-x, proc);
                        has_link = is_connected_to(peak, F, status, ident, level, u0+x, v0-y, proc);
                        has_link = is_connected_to(peak, F, status, ident, level, u0+y, v0+x, proc);
                        r = err;
                        if (r <= y) err += ++y*2+1;
                        if (r > x || err > y)
                            err += ++x*2+1;
                    } while (x < 0);

                    if(r>10) break;
                }

            }

            template <typename FIELD, typename FUNC> static inline
            bool is_connected_to(spike2D     &peak,
                                 const FIELD &F,
                                 tableau     &status,
                                 const size_t ident,
                                 const T      level,
                                 const unit_t  u,
                                 const unit_t  v,
                                 FUNC &        proc)
            {

                static const unit_t du[8] = { 0, 1, 1,  1,  0, -1, -1, -1};
                static const unit_t dv[8] = { 1, 1, 0, -1, -1, -1,  0,  1};

                const unit_t umin = status.umin;
                const unit_t umax = status.umax;
                const unit_t vmin = status.vmin;
                const unit_t vmax = status.vmax;

                // in the region ?
                if(u<umin||u>umax||v<vmin||v>vmax)
                {
                    return false;
                }

                // is occupied ?
                if(status(u,v)>0)
                {
                    return false;
                }

                // check neighbors
                T            value[8] = { 0 };

                size_t num = 0;
                for(size_t i=0;i<8;++i)
                {
                    const unit_t utmp = u+du[i];
                    const unit_t vtmp = v+dv[i];
                    if(utmp>=umin&&utmp<=umax&&vtmp>=vmin&&vtmp<=vmax)
                    {
                        if(ident==status(utmp,vtmp))
                        {
                            value[num++] = proc(F[utmp][vtmp]);
                        }
                    }
                }

                if(num<=0)
                    return false; // no neighbors

                T max_val = value[0];
                for(size_t i=1;i<num;++i)
                {
                    if(value[i]>max_val)
                    {
                        max_val = value[i];
                    }
                }


                return false;
            }
            
        };
        
        
    }
}

#endif
