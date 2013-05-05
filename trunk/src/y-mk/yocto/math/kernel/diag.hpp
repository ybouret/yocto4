#ifndef YOCTO_MATH_KERNEL_DIAG_INCLUDED
#define YOCTO_MATH_KERNEL_DIAG_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"

namespace yocto
{
    
    namespace math
    {
        
        struct eigen
        {
            static const int is_real = 0;
            static const int is_cplx = 1;
        };
        
        template <typename T>
        struct diag
        {
            //! balance a real matrix
            /**
             Given a matrix a[1..n][1..n], this routine replaces it by a balanced matrix with identical eigenvalues.
             A symmetric matrix is already balanced and is unaffected by this procedure.
             */
            static
            void HessenbergBalance( matrix<T> &a ) throw();
            
            //! reduction to a real Hessenberg form
            /**
             matrix should be balanced.
             */
            static
            void HessenbergReduce( matrix<T> &a ) throw();
            
            
            //! find the eigen values
            /**
             \param a a real matrix reduced to its Hessenberg form: destructed !
             \param wr an array that will be filled with the real parts
             \param wi an array that will be filled with the imagnary parts
             \param flag an array to know wether the value is real or cplx
             */
            static
            bool HessenbergQR( matrix<T> &a, array<T> &wr, array<T> &wi, array<int> &flag) throw();
            
            static inline
            bool eig( matrix<T> &a, array<T> &wr, array<T> &wi, array<int> &flag) throw()
            {
                HessenbergBalance(a);
                HessenbergReduce(a);
                return HessenbergQR(a, wr, wi, flag);
            }
        };
    }
    
}


#endif

