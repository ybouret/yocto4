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
		class jacobi : public object {
		public:
			explicit jacobi() throw();
			explicit jacobi( size_t n );
			virtual ~jacobi() throw();
			
			//-- a is a symetric matrix
			//-- d are the eigenvalues
			//-- v are the eigenvectors
			//-- Diag = Vt A V
			bool operator()( matrix<T> &a, array<T> &d, matrix<T> &v );
			
			//! sort eigenvalues by descending order
			//! sort columns of v accrodingly
			static
			void eigsrt( array<T> &d, matrix<T> &v ) throw();
			
		private:
			vector<T>  wksp_;
			size_t     nrot_;
		};
				
	}
}

#endif
