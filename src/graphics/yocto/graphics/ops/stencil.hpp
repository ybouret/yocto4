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
                weights            s; //!< local stencil

                explicit patch(const graphics::patch &p) throw();
                patch(const patch &p) throw();
                virtual ~patch() throw();

                template <typename T>
                void compute(lockable &) throw()
                {
                    assert(handle);
                    assert(w);

                    const pixmap<T> &pxm = *static_cast<pixmap<T> *>(handle);
                    for(unit_t j=lower.y;j<=upper.y;++j)
                    {
                        for(unit_t i=lower.x;i<=upper.x;++i)
                        {
                            stencil::load<T>(s,pxm,i,j,f);
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
                                {
                                    s(dx,dy) = real_type(p[j+dy][i+dx]);
                                }
                            }
                        }
                        break;
                        
                    default:
                        break; // todo: remove
                        assert(die("never get here"));
                }
            }

            template <typename T>
            static inline
            void launch(patches &sp, pixmap<real_type> &tgt, const weights &w, const pixmap<T> &pxm, threading::engine *server)
            {
                faked_lock access;
                for(size_t i=sp.size();i>0;--i)
                {
                    patch &sub = sp[i];
                    sub.w      = &w;
                    sub.target = &tgt;
                    sub.handle = (void*)&pxm;
                    if(server)
                    {
                        server->enqueue(&sub, &patch::compute<T>);
                    }
                    else
                    {
                        sub.compute<T>(access);
                    }
                }
                if(server) server->flush();
            }


        };
    }
}

#endif
