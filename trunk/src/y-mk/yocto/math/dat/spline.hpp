#ifndef YOCTO_MATH_DAT_SPLINE_INCLUDED
#define YOCTO_MATH_DAT_SPLINE_INCLUDED 1

#include "yocto/sequence/lw-arrays.hpp"

namespace yocto {
    
	namespace math {
        
        enum spline_type
        {
            spline_natural,       //!< both sides are natural
            spline_periodic,      //!< both sides are periodic
            spline_tangent_left,  //!< left tangent, natural at right
            spline_tangent_right, //!< right tangent, natural at left
            spline_tangent_both   //!< both tangents are set
        };
        
        template <typename T>
        struct spline
        {
            
            //! compute splines' y2
            /**
             \param  type   spline type
             \param  x      increasing coordinates
             \param  y_tab  tableau of y coordinates
             \param  y2_tab tableau of y2 values (to be computed)
             \param  ls_tab tableau of left slopes (if necessary)
             \param  rs_tab tableau of right slopes (if necessary)
             \param  ns     number of splines to be computed
             */
            static
            void compute(spline_type     type,
                         const array<T> &x,
                         const array<T> *y_tab,
                         array<T>       *y2_tab,
                         const T        *ls_tab,
                         const T        *rs_tab,
                         const size_t    ns);
            
            
            //! simulatneous splines evaluation
            /**
             \param Y array of values to be computed
             \param ns number of values
             \param X  where to compute the values
             \param y_tab the original y values
             \param y2_tab the spline coefficients
             \param width if not NULL, used for periodic conditions
             */
            static
            void eval(T              *Y,
                      const size_t    ns,
                      T               X,
                      const array<T> &x,
                      const array<T> *y_tab,
                      const array<T> *y2_tab,
                      const T        *width);
            
            
            //! 1D helper
            static
            inline void compute(spline_type     type,
                                const array<T> &x,
                                const array<T> &y,
                                array<T>       &y2,
                                const T         left_slope  = 0,
                                const T         right_slope = 0
                                )
            {
                spline<T>::compute( type, x, &y, &y2, &left_slope, &right_slope, 1);
            }
            
            //! 1D helper
            static
            inline T eval(T               X,
                          const array<T> &x,
                          const array<T> &y,
                          array<T>       &y2,
                          const T        *width)
            {
                T ans = 0;
                spline<T>::eval( &ans, 1, X, x, &y, &y2, width);
                return ans;
            }
            
        };
        
        
	}
    
}


#endif
