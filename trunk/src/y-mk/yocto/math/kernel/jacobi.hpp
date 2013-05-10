/** \file  jacobi.hpp
 */

#ifndef _YOCTO_JACOBI_HPP_
#define _YOCTO_JACOBI_HPP_ 1


#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/types.hpp"

namespace yocto {
	
	
	namespace math {
		
		template <class T>
		struct jacobi {
            
            //! Jacobi reduction
            /**
             \param a is a symetric matrix
             \param d are the eigenvalues
             \param v are the eigenvectors
             \param Diag = Vt A V
             */
            static
			bool build( matrix<T> &a, array<T> &d, matrix<T> &v );
			
			//! sort eigenvalues and according columns of v by DESCENDING order
            /**
             \param d a vector or eigenvalues
             \param v a matrix of eigenvectors
             */
			static
			void eigsrt( array<T> &d, matrix<T> &v ) throw();
			
		};
        
	}
}

#endif
