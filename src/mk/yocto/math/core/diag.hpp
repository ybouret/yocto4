#ifndef YOCTO_MATH_KERNEL_DIAG_INCLUDED
#define YOCTO_MATH_KERNEL_DIAG_INCLUDED 1

#include "yocto/math/matrix.hpp"

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
            static
            void HessenbergBalance( matrix<T> &a ) throw();
            
            //static void HessenbergReOrder( matrix<T> &a ) throw();
            
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
             \param nr the number or real eigenvalues
             wi[1..nr]=0 and wr[1..nr] are sorted by increasing order.
             */
            static
            bool HessenbergQR( matrix<T> &a, array<T> &wr, array<T> &wi, size_t &nr) throw();
            
            static inline
            bool eig( matrix<T> &a, array<T> &wr, array<T> &wi, size_t &nr) throw()
            {
                HessenbergBalance(a);
                HessenbergReduce(a);
                return HessenbergQR(a, wr, wi, nr);
            }
            
            //! find the eigenvectors from initial eigenvalues
            /**
             \param transpose eigenvectors: #rows <= number of REAL eigenvalues
             \param a initial matrix
             \param wr initial eigenvalues, 1..ev.rows are REAL. Shall be SORTED
             */
            static
            void eigv( matrix<T> &ev, const matrix<T> &a, array<T> &wr );
            
            
        };
    }
    
}


#endif

