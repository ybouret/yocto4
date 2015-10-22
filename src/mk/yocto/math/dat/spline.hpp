#ifndef YOCTO_MATH_DAT_SPLINE_INCLUDED
#define YOCTO_MATH_DAT_SPLINE_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/container/matrix.hpp"
#include "yocto/math/types.hpp"

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
                         const array<T> **ppy,
                         array<T>       **ppy2,
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
                      const array<T>  &x,
                      const array<T> **ppy,
                      const array<T> **ppy2,
                      const T        *width);
            
            //! compute the approximate derivatives array
            static
            void derivs(array<T>       **ppy1,
                        const array<T>  &x,
                        const array<T> **ppy,
                        const array<T> **ppy2,
                        const size_t     ns
                        );
            
            //! 1D helper
            static inline
            void compute(spline_type     type,
                         const array<T>  &x,
                         const array<T>  &y,
                         array<T>        &y2,
                         const T         left_slope  = 0,
                         const T         right_slope = 0
                         )
            {
                const array<T> *ppy[1]  = { &y };
                array<T>       *ppy2[1] = { &y2 };
                spline<T>::compute( type, x, ppy, ppy2, &left_slope, &right_slope, 1);
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
                const array<T> *ppy[1]  = { &y };
                const array<T> *ppy2[1] = { &y2 };
                spline<T>::eval( &ans, 1, X, x, ppy, ppy2, width);
                return ans;
            }
            
            //! 1D helper
            static inline
            void derivs(array<T>       &y1,
                        const array<T> &x,
                        const array<T> &y,
                        const array<T> &y2)
            {
                array<T>       *ppy1[1] = { &y1 };
                const array<T> *ppy[1]  = { &y  };
                const array<T> *ppy2[1] = { &y2 };
                spline<T>:: derivs(ppy1, x, ppy, ppy2, 1);
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
                assert(P.cols==t.size());
                const array<T> *y[2]  = { &P[1], &P[2] };
                array<T>      *y2[2]  = { &Q[1], &Q[2] };
                spline<T>::compute(type,
                                   t,
                                   y,
                                   y2,
                                   &LT.x,
                                   &RT.x,
                                   2);
            }
            
            //! 2D helper
            static inline
            v2d<T> eval(T u,
                        const array<T>  &t,
                        const matrix<T> &P,
                        const matrix<T> &Q,
                        const T         *width) throw()
            {
                assert(P.rows==2);
                assert(Q.rows==2);
                assert(P.cols==Q.cols);
                assert(P.cols==t.size());
                const array<T>  *y[2] = { &P[1], &P[2] };
                const array<T> *y2[2] = { &Q[1], &Q[2] };
                v2d<T> v;
                spline<T>::eval(&v.x, 2, u, t, y, y2, width);
                return v;
            }
            
            //! 2D helper
            static inline
            void derivs(matrix<T>       &D,
                        const array<T>  &t,
                        const matrix<T> &P,
                        const matrix<T> &Q )
            {
                assert(P.rows==2);
                assert(Q.rows==2);
                assert(D.rows==2);
                assert(P.cols==Q.cols);
                assert(D.cols==Q.cols);
                assert(P.cols==t.size());
                array<T>       *y1[2] = { &D[1], &D[2] };
                const array<T>  *y[2] = { &P[1], &P[2] };
                const array<T> *y2[2] = { &Q[1], &Q[2] };
                spline<T>:: derivs(y1, t, y, y2, 2);
            }
            
        protected:
            explicit spline() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(spline);
        };
        
        
        //! make a spline with own y2
        template <typename T>
        class spline1D : public spline<T>
        {
        public:
            explicit spline1D(spline_type     t,
                              const array<T> &X,
                              const array<T> &Y,
                              const T        ls=0,
                              const T        rs=0);
            
            virtual ~spline1D() throw();
            T get(const T X) const throw();
            
            inline T operator()( const T X ) const throw() { return get(X); }
            void recompute(const T ls=0,const T rs=0);
            
        private:
            const array<T>   &x;  //!< user's ref
            const array<T>   &y;  //!< user's ref
            vector<T>         y2; //!< own y2
            const T           w;     //!< width
            const T          *width; //!< &w if is_periodic
        public:
            const spline_type type;
            YOCTO_DISABLE_COPY_AND_ASSIGN(spline1D);
        };

        template <typename T>
        class spline2D : public spline<T>
        {
        public:
            virtual ~spline2D() throw();
            
            //! prepare the computation
            explicit spline2D(const array<T> &user_t);
            
            //! compute from a sequence of vertices
            /**
             \param type the spline type
             \param p    an iterator with enough points
             \param LT   left tangent vector if necessary
             \param RT   right tangent vector if necessary
             */
            template <typename ITERATOR>
            inline
            void load(spline_type      type,
                      ITERATOR         p,
                      const v2d<T>     LT = v2d<T>::zero,
                      const v2d<T>     RT = v2d<T>::zero )
            {
                for( size_t i=1;i<=n;++i,++p)
                {
                    const v2d<T> &v = *p;
                    P[1][i] = v.x;
                    P[2][i] = v.y;
                }
                width = (type == spline_periodic) ? &length : 0;
                spline<T>::compute(type,t,P,Q,LT,RT);
            }
            
            v2d<T> operator()(T X) const throw();
            
            
        private:
            const array<T>   &t;
            const size_t      n; //!< t.size()
            matrix<T>         P; //!< 2 x n
            matrix<T>         Q; //!< 2 x n
            const  T          length;
            const  T         *width;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(spline2D);
        };
        
    }
    
}


#endif
