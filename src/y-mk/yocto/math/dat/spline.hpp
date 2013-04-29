#ifndef YOCTO_MATH_DAT_SPLINE_INCLUDED
#define YOCTO_MATH_DAT_SPLINE_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/math/kernel/matrix.hpp"

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
        class spline
        {
        public:
            virtual ~spline() throw();
            
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
            
            
            //! simultaneous splines evaluation
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
            static inline
            void compute(spline_type     type,
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
            static inline
            T eval(T               X,
                   const array<T> &x,
                   const array<T> &y,
                   const array<T> &y2,
                   const T        *width)
            {
                T ans = 0;
                spline<T>::eval( &ans, 1, X, x, &y, &y2, width);
                return ans;
            }
            
            //! 2D helper
            /**
             \param type spline type
             \param t    curve parameter
             \param P    matrix: first row=x, second row=y
             \param Q    second derivative matrix
             \param LT   left tangent
             \param RT   right tangent
             */
            static inline
            void compute(spline_type      type,
                         const array<T>  &t,
                         const matrix<T> &P,
                         matrix<T>       &Q,
                         const v2d<T>     LT = v2d<T>::zero,
                         const v2d<T>     RT = v2d<T>::zero
                         )
            {
                assert(P.rows==2);
                assert(Q.rows==2);
                assert(P.cols==Q.cols);
                assert(P.cols>=2);
                const array<T> *y[2]  = { &P[1], &P[2] };
                const array<T> *y2[2] = { &Q[1], &Q[2] };
                compute(type, t, y, y2, 0, 0, 2);
            }
            
            
        protected:
            explicit spline() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(spline);
        };
        
        
        //! make a spline with own y2
        template <typename T>
        class spline1D
        {
        public:
            explicit spline1D(spline_type     t,
                              const array<T> &X,
                              const array<T> &Y,
                              const T        ls=0,
                              const T        rs=0);
            
            virtual ~spline1D() throw() {}
            
            T operator()( const T X ) const throw();
            
        private:
            const array<T> &x;
            const array<T> &y;
            vector<T>       y2;
            const T         w;
            const T        *width;
        };
        
	}
    
}


#endif
