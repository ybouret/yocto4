#ifndef YOCTO_MATH_CORE_SYMDIAG_INCLUDED
#define YOCTO_MATH_CORE_SYMDIAG_INCLUDED 1

#include "yocto/math/core/matrix.hpp"

namespace yocto
{
    namespace math
    {
        template <class T>
        struct symdiag
        {
            typedef YOCTO_MATRIX<T> matrix_t;
            typedef array<T>        array_t;

            //! Jacobi reduction
            /**
             \param a is a symetric matrix, REGENERATED at the end...
             \param d are the eigenvalues
             \param v columns are the eigenvectors
             \param Diag = Vt A V
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
        };

    }
}


#endif
