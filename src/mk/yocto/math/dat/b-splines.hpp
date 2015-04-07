#ifndef YOCTO_MK_DATA_BSPLINES_INCLUDED
#define YOCTO_MK_DATA_BSPLINES_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/code/utils.hpp"

namespace yocto {

    namespace math
    {

        template <typename U>
        struct Cubic_Bsplines
        {

            //__________________________________________________________________
            //
            // Get Points to fill segment knots
            //__________________________________________________________________
            static inline
            const U &GetPoint(  const array<U> &Points,  unit_t i ) throw()
            {
                assert(Points.size()>0);
                return Points[ clamp<unit_t>(1,i,Points.size()) ];
            }

            //__________________________________________________________________
            //
            // Return a point on the cubic b-splines: x=0: begin, x=1: end
            //__________________________________________________________________
            template <typename T> static inline
            U Get(  T x, const array<U> &Points ) throw()
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
            typedef T                     type;
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


        template <typename APPROX>
        class CubicDifferential
        {
        public:
            typedef typename APPROX::type type;

            template <typename HOST_POINTER,typename METHOD_POINTER>
            inline CubicDifferential( HOST_POINTER host, METHOD_POINTER getU, METHOD_POINTER getV ) :
            U(host,getU),
            V(host,getV),
            dS(    this, & CubicDifferential::call_dS    ),
            drvs(),
            intg()
            {
            }

            virtual ~CubicDifferential() throw() {}


            typename numeric<type>::function U;
            typename numeric<type>::function V;
            typename numeric<type>::function dS;
            derivative<type> drvs;
            integrator<type> intg;


            inline type arc_length( type a, type b )
            {
                return intg(a,b,dS,1e-4);
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(CubicDifferential);
            inline type call_dS( const type x )
            {
                static const type ftol(1e-4);
                const type dU = drvs(U,x,ftol);
                const type dV = drvs(V,x,ftol);
                return Hypotenuse(dU, dV);
            }
            
        };
        
        
    }
}

#endif
