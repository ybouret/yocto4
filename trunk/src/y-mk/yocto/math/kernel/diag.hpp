#ifndef YOCTO_MATH_KERNEL_DIAG_INCLUDED
#define YOCTO_MATH_KERNEL_DIAG_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"

namespace yocto
{
    
    namespace math
    {
        template <typename T>
        struct diag
        {
            //! balance a real matrix
            /**
             Given a matrix a[1..n][1..n], this routine replaces it by a balanced matrix with identical eigenvalues.
             A symmetric matrix is already balanced and is unaffected by this procedure.
             */
            static void balance( matrix<T> &a ) throw();
            
            //! reduction to a real Hessenberg form
            /**
             matrix should be balanced.
             */
            static void Hessenberg( matrix<T> &a ) throw();
            
            
            //! find the eigen values
            /**
             \param a a real matrix reduced to its Hessenberg form: destructed !
             */
            static void eigenvalues( matrix<T> &a, array<T> &wr, array<T> &wi );
            
        };
    }
    
}


#endif

