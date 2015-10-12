#ifndef YOCTO_MATH_KERNEL_CROUT_INCLUDED
#define YOCTO_MATH_KERNEL_CROUT_INCLUDED 1

#include "yocto/math/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {
        template <typename T>
        class crout
        {
        public:
            //! crout algorithm
            /**
             \param M a square matrix
             */
            static bool build( matrix<T> &M, bool *dneg = 0 ) throw();
            
            
            //! solve by backsubstitution
            /**
             the matrix a must have been decomposed...
             */
            static void solve(const matrix<T> &a,
                              array<T>        &b) throw();
            
            //! solve the whole matrix
            static void solve(const matrix<T>     &M,
                              matrix<T>           &Q) throw();
            
            
            //! solve the inverse matrix
            static bool inverse( matrix<T> &a );
            
            
            //! Moore-Penrose pseudo inverse, P.cols >= P.rows
            static bool pseudo_inverse(matrix<T> &M, const matrix<T> &P);
            
            //! improve x for A*x = b
            static void improve( array<T> &x, const matrix<T> &A, const matrix<T> &ALU, const array<T> &b ) throw();
            
        private:
            
        };
        
        
    }
}

#endif

