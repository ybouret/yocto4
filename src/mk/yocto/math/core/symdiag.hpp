#ifndef YOCTO_MATH_CORE_SYMDIAG_INCLUDED
#define YOCTO_MATH_CORE_SYMDIAG_INCLUDED 1

#include "yocto/container/matrix.hpp"

namespace yocto
{
    namespace math
    {
        template <class T>
        struct symdiag
        {
            typedef matrix<T> matrix_t;
            typedef array<T>  array_t;

            //! Jacobi reduction
            /**
             \param a is a symetric matrix, REGENERATED at the end...
             \param d are the eigenvalues
             \param v columns are the eigenvectors
             \param Diag = V'* A * V
             */
            static
            bool build( matrix_t &a, array_t &d, matrix_t &v );

            //! sort eigenvalues and according columns of v by DESCENDING order
            /**
             \param d a vector or eigenvalues
             \param v a matrix of eigenvectors
             */
            static
            void eigsrt( array_t &d, matrix_t &v ) throw();

            //! sort eigenvalues and according columns of v by AMPLUTUDE DESCENDING order
            /**
             \param d a vector or eigenvalues
             \param v a matrix of eigenvectors
             */
            static
            void eigsrtA( array_t &d, matrix_t &v ) throw();

            //! take the inverse of the eigenvalues, with conditioning.
            /**
             \param d a vector of eignevalues
             compute tol = epsilon * n * max(|d|).
             set d[i] to to its inverse if |d[i]|>tol, 0 otherwise.
             \return the number of 0 eigenvalues, a.k.a the numeric kernel size.
             */
            static
            size_t eiginv( array_t &d ) throw();

            //! compute b = V*diag(d)*V'.
            static
            void  compute( matrix_t &b, const array_t &d, const matrix_t &v);
        };

    }
}


#endif
