#ifndef YOCTO_GFX_OPS_STENCIL_INCLUDED
#define YOCTO_GFX_OPS_STENCIL_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/xpatch.hpp"
#include "yocto/container/tuple.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace gfx
    {

        class stencil
        {
        public:
            YOCTO_PAIR_DECL(YOCTO_TUPLE_STANDARD,mask,vertex,r,float,weight);
            YOCTO_PAIR_END();

            virtual ~stencil() throw();
            explicit stencil(const size_t n);
            stencil(const stencil &other);

            const size_t size;
            const mask & operator[](const size_t i) const throw();
            void optimize() throw();
            void display() const;
            
            template <typename T, typename U>
            inline void apply(pixmap<T>       &target,
                              const pixmap<U> &source,
                              xpatches        &xps)
            {
                tgt = &target;
                src = &source;
                xps.submit(this, &stencil::run<T,U>);
            }


#define YGFX_STENCIL_LOOP(I)              \
const mask   &msk = masks[I];             \
const vertex  pos = center + msk.r;       \
if(source.has(pos))                       \
sum += float(source[pos])*msk.weight;     \
else                                      \
sum += v0*msk.weight


            template <typename T,typename U>
            inline T dot(const pixmap<U> &source,
                         const vertex    &center) throw()
            {
                assert(size>0);
                assert(source.has(center));
                const float v0  = float(source[center]);
                float       sum = 0.0f;
                YOCTO_LOOP_FUNC_(size,YGFX_STENCIL_LOOP,0);
                return T(sum);
            }

        private:
            YOCTO_DISABLE_ASSIGN(stencil);
            size_t       nmask;
        protected:
            mask        *masks;
        private:
            void        *tgt;
            const void  *src;

            template <typename T,typename U>
            inline void run( xpatch &xp, lockable & ) throw()
            {
                assert(tgt);
                assert(src);
                pixmap<T>       &target = *static_cast<pixmap<T>      *>(tgt);
                const pixmap<U> &source = *static_cast<const pixmap<U>*>(src);
                vertex v;
                for(v.y=xp.upper.y;v.y>=xp.lower.y;--v.y)
                {
                    for(v.x=xp.upper.x;v.x>=xp.lower.x;--v.x)
                    {
                        target[v] = dot<T,U>(source,v);
                    }
                }
            }
            
        };


        class stencil_grad_x : public stencil
        {
        public:
            explicit stencil_grad_x();
            virtual ~stencil_grad_x() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_grad_x);
        };


        class stencil_grad_y : public stencil
        {
        public:
            explicit stencil_grad_y();
            virtual ~stencil_grad_y() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_grad_y);
        };


        class stencil_sobel_x : public stencil
        {
        public:
            explicit stencil_sobel_x();
            virtual ~stencil_sobel_x() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_sobel_x);
        };

        class stencil_sobel_y : public stencil
        {
        public:
            explicit stencil_sobel_y();
            virtual ~stencil_sobel_y() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_sobel_y);
        };

        class stencil_gauss : public stencil
        {
        public:
            explicit stencil_gauss(const size_t w, const float sig);
            virtual ~stencil_gauss() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(stencil_gauss);
        };


    }
}

#endif
