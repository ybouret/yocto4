#ifndef YOCTO_MK_DATA_BSPLINES_INCLUDED
#define YOCTO_MK_DATA_BSPLINES_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/fcn/drvs.hpp"

namespace yocto {
    
    namespace math
    {

        template <typename U>
        struct Cubic_Bsplines
        {

            static inline const U &GetPoint(  const array<U> &Points,  unit_t i ) throw()
            {
                assert(Points.size()>0);
                const unit_t num_points = Points.size();

                if(i<=1)
                {
                    i=1;
                }
                else
                {
                    if(i>=num_points)
                    {
                        i = num_points;
                    }
                    else
                    {

                    }
                }
                return Points[i];

            }

            template <typename T> static inline
            U Get( const T x, const array<U> &Points ) throw()
            {
                const unit_t num_points   = Points.size();
                const unit_t num_segments = num_points + 1;
                unit_t start_cv = -2;


                // x: 0 -> 1: find the segment
                const T      xscaled = x*num_segments;
                const unit_t segment = Floor( xscaled );
                const T      t       = (xscaled-segment);

                // prepare points
                start_cv += segment;

                const T t2 = t*t;
                const T it = T(1.0)-t;
                const T t3 = t * t2;

                // calculate blending functions for cubic bspline
                const T b0 = it*it*it/T(6.0);
                const T b1 = (3*t3 - 6*t2 + T(4.0))/T(6.0);
                const T b2 = (-3*t3 +3*t2 + 3*t + T(1.0))/T(6.0);
                const T b3 =  t3/T(6.0);

                return
                b0 * GetPoint(Points,start_cv+1) +
                b1 * GetPoint(Points,start_cv+2) +
                b2 * GetPoint(Points,start_cv+3) +
                b3 * GetPoint(Points,start_cv+4);

            }



        };

        template <
        typename T,
        template  <typename> class VTX
        >
        class CubicApproximation : public vector< VTX<T> >
        {
        public:

            typedef VTX<T>                vtx_t;
            typedef vector<vtx_t>         vector_type;
            typedef Cubic_Bsplines<vtx_t> approx_type;

            explicit CubicApproximation() throw() :
            vector_type() {}
            virtual ~CubicApproximation() throw()  {}
            inline   CubicApproximation(const CubicApproximation &other) : vector_type(other) {}


            inline vtx_t Compute( const T x ) const throw()
            {
                return approx_type::template Get<T>(x,*this);
            }

            inline T X( const T x ) const throw()
            {
                return approx_type::template Get<T>(x,*this).x;
            }

            inline T Y( const T x ) const throw()
            {
                return approx_type::template Get<T>(x,*this).y;
            }

            inline T Z( const T x ) const throw()
            {
                return approx_type::template Get<T>(x,*this).z;
            }


        private:
            YOCTO_DISABLE_ASSIGN(CubicApproximation);

        };

    }
}

#endif
