#ifndef YOCTO_MK_DATA_BSPLINES_INCLUDED
#define YOCTO_MK_DATA_BSPLINES_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/fcn/drvs.hpp"

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
            const T      xscaled = clamp<T>(0,x,1)*num_segments;
            const unit_t segment = Floor( xscaled );
            const T      t       = (x*num_segments-segment);

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



        template <
        typename T,
        template  <typename> class VTX
        >
        class CubicApproximation : public vector< VTX<T> >
        {
        public:

            typedef T                             type;
            typedef VTX<T>                        vtx_t;
            typedef vector<vtx_t>                 vec_t;
            typedef typename numeric<T>::function fcn_t;
            static const size_t                   DIM = sizeof(vtx_t)/sizeof(type);

            fcn_t                   dArc;
            mutable derivative<T>   drvs;

            explicit CubicApproximation() :
            vec_t(),
            dArc(  this, & CubicApproximation:: d_length  ),
            coord( this, & CubicApproximation:: get_coord ),
            indx(0)
            {}

            virtual ~CubicApproximation() throw()  {}

            inline   CubicApproximation(const CubicApproximation &other) :
            vec_t(other),
            dArc(  this, & CubicApproximation:: d_length  ),
            coord( this, & CubicApproximation:: get_coord ),
            indx(0)
            {}


            inline vtx_t Compute( const T x ) const throw()
            {
                return Cubic_Bsplines(x,*this);
            }

            inline vtx_t Tangent( const T x ) const throw()
            {
                // optimal step for x around 0.5
                static const T h_opt = Pow(numeric<T>::epsilon,T(1.0)/T(3.0))*T(0.5);
                vtx_t q;
                type *p = (type *)&q;
                for(indx=0;indx<DIM;++indx)
                {
                    p[indx] = drvs(coord,x,h_opt);
                }
                return q;
            }


        private:
            YOCTO_DISABLE_ASSIGN(CubicApproximation);
            mutable fcn_t  coord;
            mutable size_t indx; //!< to get derivative

            inline T get_coord( const T x )
            {
                assert(indx<DIM);
                const vtx_t p = Compute(x);
                const T    *q = (const T *)&p;
                return      q[indx];
            }

            inline T d_length( const T x ) throw()
            {
                const vtx_t q = Tangent(x);
                T sum(0);
                const T *p = (const T*)&q;
                for(size_t i=0;i<DIM;++i)
                {
                    sum += Square(p[i]);
                }
                return Sqrt(sum);
            }
            
        };
        
        
        
        
    }
}

#endif
