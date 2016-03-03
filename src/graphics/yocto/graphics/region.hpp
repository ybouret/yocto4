#ifndef YOCTO_GRAPHICS_REGION_INCLUDED
#define YOCTO_GRAPHICS_REGION_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/code/static-check.hpp"

namespace yocto
{
    namespace graphics
    {

        //! region extended element=regxel
        class regxel
        {
        public:
            vertex r;      //!< sizeof: 2*sizeof(unit_t)
            unit_t flag;   //!< sizeof: 1*sizeof(unit_t)
            char   data[3*sizeof(unit_t)];

            template <typename T>
            inline T &get() throw()
            {
                YOCTO_STATIC_CHECK(sizeof(T)<=sizeof(data),type_size_too_big);
                return *(T *)&data[0];
            }

            template <typename T>
            inline const T &get() const throw()
            {
                YOCTO_STATIC_CHECK(sizeof(T)<=sizeof(data),type_size_too_big);
                return *(const T *)&data[0];
            }

            regxel() throw();
            ~regxel() throw();
            regxel(const vertex &p) throw();
            regxel(const regxel &other) throw();


            static inline int fast_compare(const regxel &lhs, const regxel &rhs) throw()
            {
                return gist::fast_compare(lhs.r, rhs.r);
            }

            YOCTO_DISABLE_ASSIGN(regxel);


        };

    }

}

YOCTO_SUPPORT_C_STYLE_OPS(graphics::regxel);

#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace graphics
    {

        typedef vector<regxel> regxels;

        class region : public regxels
        {
        public:
            explicit region() throw();
            virtual ~region() throw();
            region(const region &);

            void simplify() throw();
            
            void load_square(const unit_t r);
            void load_disk(const unit_t r);
            void shift(const unit_t x, const unit_t y) throw();
            void center() throw();

            template <typename T>
            inline void tag(pixmap<T>   &pxm,
                            const unit_t xc,
                            const unit_t yc,
                            typename pixmap<T>::param_type value) const
            {
                const vertex c(xc,yc);
                const regxels &self = *this;
                for(size_t i=size();i>0;--i)
                {
                    const vertex p = self[i].r+c;
                    if(pxm.has(p))
                    {
                        pxm[p] = value;
                    }
                }
            }

#if 0
            template <typename T>
            inline size_t load(const pixmap<T> &pxm,
                               const unit_t     xc,
                               const unit_t     yc)
            {
                const vertex c(xc,yc);
                size_t count = 0;
                for(node_type *node=list_.head;node;node=node->next)
                {
                    regxel       &rg = node->data;
                    const vertex p = rg.r + c;
                    if(pxm.has(p))
                    {
                        rg.flag = true;
                        ++count;
                        rg.get<T>() = pxm[p];
                    }
                    else
                    {
                        rg.flag = false;
                    }

                }
            }
#endif
            

        private:
            YOCTO_DISABLE_ASSIGN(region);
            

        };

    }
}

#endif

