#ifndef YOCTO_GFX_OPS_PARTICLES_INCLUDED
#define YOCTO_GFX_OPS_PARTICLES_INCLUDED 1

#include "yocto/gfx/ops/tagmap.hpp"

namespace yocto
{
    namespace gfx
    {
        //! one particle is a list of vertices
        class particle : public counted_object, public vlist
        {
        public:
            typedef arc_ptr<particle> ptr;

            explicit particle(const size_t id) throw();
            virtual ~particle() throw();
            const size_t tag;
            vlist        inside;
            vlist        border;

            template <typename T>
            inline void mask( pixmap<T> &src, const T C, const uint8_t alpha) const throw()
            {
                mask_list(*this,src,C,alpha);
            }

            template <typename T>
            inline void mask_inside( pixmap<T> &src, const T C, const uint8_t alpha) const throw()
            {
                mask_list(inside,src,C,alpha);
            }

            template <typename T>
            inline void mask_border( pixmap<T> &src, const T C, const uint8_t alpha) const throw()
            {
                mask_list(border,src,C,alpha);
            }


            template <typename T>
            inline void transfer(pixmap<T>       &tgt,
                                 const pixmap<T> &src)  throw()
            {
                transfer_list(*this,tgt,src);
            }

            template <typename T>
            inline void transfer_border(pixmap<T>       &tgt,
                                        const pixmap<T> &src)  throw()
            {
                transfer_list(border,tgt,src);
            }


            template <typename T>
            inline void transfer_inside(pixmap<T>       &tgt,
                                        const pixmap<T> &src)  throw()
            {
                transfer_list(inside,tgt,src);
            }



            void regroup() throw();
            void split_using( const tagmap &tags ) throw();

            //! width of a regouped particle
            vertex width() const throw();


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(particle);
            template <typename T>
            static inline void mask_list(const vlist  &L,
                                         pixmap<T>    &src,
                                         const T       C,
                                         const uint8_t alpha)  throw()
            {
                for(const vnode *node=L.head;node;node=node->next)
                {
                    assert(src.has(node->vtx));
                    T &S = src[node->vtx];
                    S    = pixel<T>::blend(S,C,alpha);
                }
            }

            template <typename T>
            static inline void transfer_list(const vlist     &L,
                                             pixmap<T>       &tgt,
                                             const pixmap<T> &src)  throw()
            {
                for(const vnode *node=L.head;node;node=node->next)
                {
                    assert(src.has(node->vtx));
                    assert(tgt.has(node->vtx));
                    tgt[node->vtx] = src[node->vtx];
                }
            }


        };

        //! base class for particles
        typedef vector<particle::ptr> _particles;

        //! particles is a vector or particle pointers.
        class particles : public _particles
        {
        public:
            explicit particles() throw();
            virtual ~particles() throw();
            
            //! sort by decreasing size
            void sort() throw();
            
            void load(const tagmap &tags);

            void regroup_all() throw();
            void split_all_using(const tagmap &tags) throw();

            //! remove particles with empty inside
            void remove_shallow_with(tagmap &tags) throw();


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(particles);
        };
        
        
    }
}


#endif
