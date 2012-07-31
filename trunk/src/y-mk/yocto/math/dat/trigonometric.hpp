#ifndef YOCTO_MATH_TRIGONOMETRIC_INCLUDED
#define YOCTO_MATH_TRIGONOMETRIC_INCLUDED 1

#include "yocto/math/kernel/linsys.hpp"


namespace yocto
{
    
    namespace math
    {
        
        //! trigonometric interpolation
        template <typename T>
        class trigonometric
        {
        public:
            //! build the resources
            /**
             \param theta in 0..2*pi exclusive
             \param solver to perform the LU on the matrix
             */
            explicit trigonometric( const array<T> &theta, linsys<T> &solver );
            virtual ~trigonometric() throw();
            
            //! compute the trigonometric coefficicents
            /**
             \param a the values at points theta from the constructor
             \param solver the untouched solver used in the constructor
             The values are replaced by the coefficients.
             */
            void compute( array<T> &a, linsys<T> &solver ) const throw();
            
            T operator()( T theta, const array<T> &a ) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(trigonometric);
            const size_t n;       //!< theta.size
            const bool   is_even; //!< is n is even
            const size_t n2;      //!< n over 2
            const size_t nn;      //!< n2, n2-1 if is_even
            matrix<T>    M;
        };
        
    }
    
}

#endif
