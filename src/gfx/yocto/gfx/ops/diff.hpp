#ifndef YOCTO_GFX_OPS_DIFF_INCLUDED 
#define YOCTO_GFX_OPS_DIFF_INCLUDED 1

#include "yocto/gfx/pixel.hpp"
#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/xpatch.hpp"
#include "yocto/code/bzset.hpp"

namespace yocto
{
    namespace gfx
    {

        class difference
        {
        public:
            inline explicit difference() throw() : lhs(0), rhs(0) {}
            inline virtual ~difference() throw() {}

            template <typename T>
            inline float L2( const pixmap<T> &source, xpatches &xps )
            {
                lhs = &source;
                const size_t np = xps.size();
                for(size_t i=np;i>0;--i)
                {
                    xpatch &xp = xps[i];
                    xp.make<float>() = 0;
                    xp.enqueue(this, & difference::run1<T>, xps.server);
                }
                xps.server->flush();

                float  sum = 0;
                for(size_t i=np;i>0;--i)
                {
                    sum += xps[i].as<float>();
                }
                return sum;
            }

            template <typename T>
            inline float L2( const pixmap<T> &LHS, const pixmap<T> &RHS, xpatches &xps )
            {
                lhs = &LHS;
                rhs = &RHS;
                const size_t np = xps.size();
                for(size_t i=np;i>0;--i)
                {
                    xpatch &xp = xps[i];
                    xp.make<float>() = 0;
                    xp.enqueue(this, & difference::run2<T>, xps.server);
                }
                xps.server->flush();

                float  sum = 0;
                for(size_t i=np;i>0;--i)
                {
                    sum += xps[i].as<float>();
                }
                return sum;
            }
            


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(difference);
            const void *lhs;
            const void *rhs;

            template <typename T> inline
            void run1( xpatch &xp, lockable & ) throw()
            {
                assert(lhs);
                const unit_t     ymin = xp.lower.y;
                const unit_t     ymax = xp.upper.y;
                const unit_t     xmin = xp.lower.x;
                const unit_t     xmax = xp.upper.x;
                const pixmap<T> &LHS  = *static_cast<const pixmap<T> *>(lhs);
                T __zero;
                bzset(__zero);
                float localSum = 0;
                for(unit_t y=ymax;y>=ymin;--y)
                {
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        localSum += pixel<T>::L2(LHS[y][x],__zero);
                    }
                }
                xp.as<float>() = localSum;
            }

            template <typename T> inline
            void run2( xpatch &xp, lockable & ) throw()
            {
                assert(lhs);
                assert(rhs);
                const unit_t     ymin = xp.lower.y;
                const unit_t     ymax = xp.upper.y;
                const unit_t     xmin = xp.lower.x;
                const unit_t     xmax = xp.upper.x;
                const pixmap<T> &LHS  = *static_cast<const pixmap<T> *>(lhs);
                const pixmap<T> &RHS  = *static_cast<const pixmap<T> *>(rhs);

                float localSum = 0;
                for(unit_t y=ymax;y>=ymin;--y)
                {
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        localSum += pixel<T>::L2(LHS[y][x],RHS[y][x]);
                    }
                }
                xp.as<float>() = localSum;
            }


        };

    }
}

#endif
