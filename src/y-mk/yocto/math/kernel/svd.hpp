#ifndef YOCTO_MATH_SVD_INCLUDED
#define YOCTO_MATH_SVD_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"

namespace yocto
{
    namespace math
    {
        
        template <typename T>
        struct svd
        {
            //! singular value decomposition
            /**
             Given a matrix A[1..m][1..n], this routine computes its singular value
             decomposition, A = U.W.VT.  The matrix U replaces a on output.  The diagonal
             matrix of singular values W is output as a vector w[1..n].  The matrix V (not
             the transpose VT) is output as v[1..n][1..n].
             */
            static bool build(matrix<T> &A, array<T> &W, matrix<T> &V);
            
            //! solve a system after a singular value decomposition
            /**
             Solves A·X = B for a vector X, where A is specified
             by the arrays u[1..m][1..n], w[1..n], v[1..n][1..n] as returned by svdcmp.
             m and n are the dimensions of a,
             and will be equal for square matrices.
             b[1..m] is the input right-hand side.
             x[1..n] is the output solution vector.
             No input quantities are destroyed, so the routine may be called sequentially with different b’s.
             */
            static
            void solve(const matrix<T> &u,
                       const array<T>  &w,
                       const matrix<T> &v,
                       const array<T>  &b,
                       array<T>        &x);
          
            //! truncate singular value to ftol * max|w|
            /**
             \return true if a value was truncated
             */
            static
            bool truncate( array<T> &w, const T ftol );
            
        };
        
    }
}


#endif
