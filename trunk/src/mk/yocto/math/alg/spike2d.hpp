#ifndef YOCTO_MATH_ALG_SPIKE2D_INCLUDED
#define YOCTO_MATH_ALG_SPIKE2D_INCLUDED 1

#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/core/list.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/code/utils.hpp"


#include <iostream>

namespace yocto
{
    namespace math
    {

        class spike2d : public object
        {
        public:
            typedef arc_ptr<spike2d> pointer;

            typedef v2d<unit_t> vtx;

            class coord
            {
            public:
                coord *next;
                coord *prev;
                const unit_t x,y;
                YOCTO_MAKE_OBJECT
                inline coord(const unit_t X, const unit_t Y) throw():
                x(X), y(Y)
                {
                };

                inline ~coord() throw() {}

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(coord);
            };

            typedef core::list_of_cpp<coord> coord_list;

            const vtx    position;
            const double value;
            coord_list   coords; //!< inside coords
            coord_list   border; //!< boorder only
            const unit_t uuid;   //!< id


            template <
            typename FIELD,
            typename COORD >
            static void detect(const FIELD &field,
                               const COORD  xlo,
                               const COORD  xhi,
                               const COORD  ylo,
                               const COORD  yhi)
            {
                detector<FIELD,COORD> scan(field,xlo,xhi,ylo,yhi);

            }



        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(spike2d);
            template <
            typename FIELD,
            typename COORD>
            class detector
            {
            public:
                const vtx      lo;
                const vtx      hi;
                const unit_t   n;
                const unit_t   m;
                const unit_t   q;
                matrix<unit_t> flag;
                vector<size_t> indx;
                vector<double> val;
                vector<vtx>    pos;
                unit_t         uuid;

                inline detector(const FIELD &field,
                                const COORD  xlo,
                                const COORD  xhi,
                                const COORD  ylo,
                                const COORD  yhi ) :
                lo(min_of(xlo,xhi),min_of(ylo,yhi)),
                hi(max_of(xlo,xhi),max_of(ylo,yhi)),
                n(hi.x - lo.x + 1),
                m(hi.y - lo.y + 1),
                q(n*m),
                flag(n,m),
                indx(q,as_capacity),
                val(q,as_capacity),
                pos(q,as_capacity),
                uuid(0)
                {
                    // flag is initialized to 0

                    // initialize values, positions
                    vtx v;
                    for(v.x=lo.x;v.x<=hi.x;++v.x)
                    {
                        for(v.y=lo.y;v.y<=hi.y;++v.y)
                        {
                            val.push_back( field[v.x][v.y] );
                            pos.push_back( v );
                        }
                    }

                    // order value
                    co_qsort(val, pos, __compare_decreasing<double>);

                    
                }
                
                
            };
        };
        
    }
}

#endif
