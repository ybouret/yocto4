#ifndef YOCTO_GRAPHICS_OPS_PARTICLES_INCLUDED
#define YOCTO_GRAPHICS_OPS_PARTICLES_INCLUDED 1

#include "yocto/graphics/ops/tagmap.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace graphics
    {

        //! a particle is a list of vertices
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

            vertex compute_extension() throw();

            vnode_list inside;
            vnode_list border;

            void regroup() throw();

            void split_using(const tagmap &tmap ) throw();

            template <typename T>
            void transfer_with_contour(pixmap<T>       &tgt,
                                       const pixmap<T> &src,
                                       const T contour_value) const
            {
                for(const vnode_type *node = inside.head; node; node=node->next)
                {
                    const vertex v = node->vtx;
                    assert(tgt.has(v));
                    tgt[v] = src[v];
                }

                for(const vnode_type *node = border.head; node; node=node->next)
                {
                    const vertex v = node->vtx;
                    assert(tgt.has(v));
                    tgt[v] = contour_value;
                }

            }

            //! dilate using borders, split if necessary
            /**
             the points are regrouped
             \return the number of extra points
             */
            size_t dilate_with( tagmap &tmap ) throw();


            //! one border in common...
            bool touches( const particle &other ) const throw();

        private:
            YOCTO_DISABLE_ASSIGN(particle);
        };

        //! base class for particles
        typedef vector<particle::ptr> _particles;


        //! to be used with tagmap
        class particles : public _particles
        {
        public:
            explicit particles() throw();
            virtual ~particles() throw();
            void sort() throw();

            void load( const tagmap &tmap );

            void fusion( tagmap &tmap);

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(particles);
        };


    }
}

#endif
