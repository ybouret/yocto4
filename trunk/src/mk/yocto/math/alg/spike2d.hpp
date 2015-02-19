#ifndef YOCTO_MATH_ALG_SPIKE2D_INCLUDED
#define YOCTO_MATH_ALG_SPIKE2D_INCLUDED 1

#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/container/sequence.hpp"
#include "yocto/core/list.hpp"

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

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(spike2D);
        };


        template <typename T, typename COORD>
        struct spike2D_finder
        {
            typedef spike2D<T,COORD>             spike_type;
            typedef typename spike_type::pointer spike_ptr;

            class tableau
            {
            public:
                const COORD  umin;
                const COORD  umax;
                const size_t nu;
                const COORD  vmin;
                const COORD  vmax;
                const size_t nv;
                const size_t items;

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
                wlen(items),
                flag( memory::kind<memory::global>::acquire_as<bool>(wlen) )
                {
                    for(size_t i=items;i>0;--i) flag[i] = true;
                }

                inline ~tableau() throw()
                {
                    memory::kind<memory::global>::release_as<bool>(flag, wlen);
                }


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(tableau);
                size_t wlen;
                bool  *flag;

            };

            template <typename FIELD>
            static inline void detect(sequence<spike_ptr> &spikes,
                                      const FIELD         &F,
                                      const COORD          umin,
                                      const COORD          umax,
                                      const COORD          vmin,
                                      const COORD          vmax
                                      )
            {
                assert(umax>=umin);
                assert(vmax>=vmin);
                spikes.free();

                //! tableau of access
                tableau flag(umin,umax,vmin,vmax);

                
            }
        };

    }
}

#endif
