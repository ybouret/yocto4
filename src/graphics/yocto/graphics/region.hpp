#ifndef YOCTO_GRAPHICS_REGION_INCLUDED
#define YOCTO_GRAPHICS_REGION_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    namespace graphics
    {

        typedef list<vertex> region_base;

        class region : public region_base
        {
        public:
            explicit region() throw();
            virtual ~region() throw();
            region(const region &);
            region & operator=(const region &);

            void simplify() throw();
            
            void load_square(const unit_t r);
            void load_disk(const unit_t r);
            void shift(const unit_t x, const unit_t y) throw();

            template <typename T>
            inline void tag(pixmap<T> &pxm,
                            const unit_t xc,
                            const unit_t yc,
                            typename pixmap<T>::param_type value) const
            {
                const vertex c(xc,yc);
                for(const node_type *node = list_.head;node;node=node->next)
                {
                    const vertex p = node->data + c;
                    if(pxm.has(p))
                    {
                        pxm[p] = value;
                    }
                }
            }

            

        };

    }
}

#endif

