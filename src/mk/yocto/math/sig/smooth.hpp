#ifndef YOCTO_MATH_SIG_SMOOTH
#define YOCTO_MATH_SIG_SMOOTH 1

#include "yocto/math/sig/expand.hpp"
#include "yocto/math/point2d.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace math
    {

        template <typename T>
        class smooth
        {
        public:
            typedef point2d<T> point_t;

            inline explicit smooth() throw() : points(), lower_range(0), upper_range(0), degree(0) {}
            inline virtual ~smooth() throw() {}

            list<point_t> points;
            T             lower_range; //!< how far before current value
            T             upper_range; //!< how far beyond current value
            size_t        degree;      //!< desired degreee

            inline T operator()(const expand<T> &xp,
                                const unit_t     i,
                                const array<T>  &X,
                                const array<T>  &Y)
            {
                assert(X.size()>0);
                assert(X.size()==Y.size());
                const unit_t N = X.size();

                //______________________________________________________________
                //
                // prepare with central point
                //______________________________________________________________
                const T Xc = xp.get_x(i,X,N);
                points.free();
                push_back(Xc, xp.get_y(i,Y,N));
                //std::cerr << "points=" << points << std::endl;
                //______________________________________________________________
                //
                // add next points
                //______________________________________________________________
                {
                    const T Xup = Xc + max_of<T>(0,upper_range);
                    unit_t  iup = i;
                    while(true)
                    {
                        const T Xi=xp.get_x(++iup,X,N);
                        if(Xi>Xup) break;
                        push_back(Xi-Xc,xp.get_y(iup,Y,N));
                    }
                }

                //______________________________________________________________
                //
                // add prev points
                //______________________________________________________________
                {
                    const T Xdn = Xc - max_of<T>(0,lower_range);
                    unit_t  idn = i;
                    while(true)
                    {
                        const T Xi=xp.get_x(--idn,X,N);
                        if(Xi<Xdn) break;
                        push_front(Xi-Xc,xp.get_y(idn,Y,N));
                    }
                }
                std::cerr << "points=" << points << std::endl;
                return 0;
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(smooth);
            inline void push_back(  T x, T y ) { const point_t p(x,y); points.push_back(p);  }
            inline void push_front( T x, T y ) { const point_t p(x,y); points.push_front(p); }
        };
    }
}


#endif
