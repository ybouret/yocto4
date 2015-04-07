#ifndef YOCTO_MK_DATA_BSPLINES_INCLUDED
#define YOCTO_MK_DATA_BSPLINES_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/utils.hpp"

namespace yocto {

    namespace math
    {




        //______________________________________________________________________
        //
        // Return a point on the cubic b-splines: x=0: begin, x=1: end
        //______________________________________________________________________
        template <typename T,typename U>
        inline U Cubic_Bsplines( T x, const array<U> &Points ) throw()
        {
            const unit_t num_points   = Points.size();
            const unit_t num_segments = num_points + 1;
            unit_t start_cv = -2;

            //______________________________________________________________
            //
            // x: 0 -> 1: find the segment
            //______________________________________________________________
            x = clamp<T>(0,x,1);
            const T      xscaled = x*num_segments;
            const unit_t segment = Floor( xscaled );
            const T      t       = (xscaled-segment);

            //______________________________________________________________
            //
            // prepare points
            //______________________________________________________________
            start_cv += segment;

            const T t2 = t*t;
            const T it = T(1.0)-t;
            const T t3 = t * t2;

            //______________________________________________________________
            //
            // calculate blending functions for cubic bspline
            //______________________________________________________________
            const T b0 = it*it*it/T(6.0);
            const T b1 = (3*t3 - 6*t2 + T(4.0))/T(6.0);
            const T b2 = (-3*t3 +3*t2 + 3*t + T(1.0))/T(6.0);
            const T b3 =  t3/T(6.0);

            return
            b0 * Points[clamp<unit_t>(1,start_cv+1,num_points)] +
            b1 * Points[clamp<unit_t>(1,start_cv+2,num_points)] +
            b2 * Points[clamp<unit_t>(1,start_cv+3,num_points)] +
            b3 * Points[clamp<unit_t>(1,start_cv+4,num_points)];
        }

        //______________________________________________________________________
        //
        // Tangent vector approximation
        //______________________________________________________________________
        template <typename T,typename U>
        inline U Cubic_Bsplines_Tangent( T x, const array<U> &Points ) throw()
        {
            //! scaling for x=0.5
            static const T h_opt = Pow(numeric<T>::epsilon,T(1.0)/T(3.0))*T(0.5);

            volatile T tmp = x + h_opt;
            const    T h   = tmp-x;
            const    U lo  = Cubic_Bsplines<T,U>(x-h,Points);
            const    U hi  = Cubic_Bsplines<T,U>(x+h,Points);
            return (T(1.0)/(h+h)) * (hi-lo);
        }



        template <
        typename T,
        template  <typename> class VTX
        >
        class CubicApproximation : public vector< VTX<T> >
        {
        public:
            typedef T                     type;
            typedef VTX<T>                vtx_t;
            static const size_t           DIM = sizeof(vtx_t)/sizeof(type);
            typedef vector<vtx_t>         vector_type;

            typename numeric<T>::function dS;

            explicit CubicApproximation() :
            vector_type(),
            dS( this, & CubicApproximation:: dLength )
            {}
            virtual ~CubicApproximation() throw()  {}
            inline   CubicApproximation(const CubicApproximation &other) :
            vector_type(other),
            dS(other.dS)
            {}


            inline vtx_t Compute( const T x ) const throw()
            {
                return Cubic_Bsplines(x,*this);
            }

            inline vtx_t Tangent( const T x ) const throw()
            {
                return Cubic_Bsplines_Tangent(x,*this);
            }

            inline T X( const T x ) const throw()
            {
                return Cubic_Bsplines(x,*this).x;
            }

            inline T Y( const T x ) const throw()
            {
                return Cubic_Bsplines(x,*this).y;
            }

            inline T Z( const T x ) const throw()
            {
                return Cubic_Bsplines(x,*this).z;
            }

        private:
            YOCTO_DISABLE_ASSIGN(CubicApproximation);

            inline T dLength( const T x ) const throw()
            {
                const vtx_t Q = Cubic_Bsplines_Tangent(x,*this);
                T sum(0);
                const T *q = (const T *)&Q;
                for(size_t i=0;i<DIM;++i)
                {
                    sum += Square(q[i]);
                }
                return Sqrt(sum);
            }

        };


        
        
    }
}

#endif
