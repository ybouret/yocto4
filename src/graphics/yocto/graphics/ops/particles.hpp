#ifndef YOCTO_GRAPHICS_OPS_PARTICLES_INCLUDED
#define YOCTO_GRAPHICS_OPS_PARTICLES_INCLUDED 1

#include "yocto/graphics/ops/tagmap.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace graphics
    {

        class particle : public counted_object, public vnode_list
        {
        public:
            typedef arc_ptr<particle> ptr;

            explicit particle(const size_t t) throw();
            virtual ~particle() throw();
            particle(const particle &other);

            const size_t tag;
            
            //! transfer part of the image
            template <typename T>
            void transfer( pixmap<T> &tgt, const pixmap<T> &src) const
            {
                for(const vnode_type *node = head; node; node=node->next)
                {
                    const vertex v = node->vtx;
                    assert(tgt.has(v));
                    assert(src.has(v));
                    tgt[v] = src[v];
                }
            }

            template <typename T>
            void transfer( pixmap<T> &tgt, const T value) const
            {
                for(const vnode_type *node = head; node; node=node->next)
                {
                    const vertex v = node->vtx;
                    assert(tgt.has(v));
                    tgt[v] = value;
                }
            }


        private:
            YOCTO_DISABLE_ASSIGN(particle);
        };

        typedef vector<particle::ptr> _particles;


        class particles : public _particles
        {
        public:
            explicit particles() throw();
            virtual ~particles() throw();
            void sort() throw();

            void load( const tagmap &tmap );

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(particles);
        };


    }
}

#endif
