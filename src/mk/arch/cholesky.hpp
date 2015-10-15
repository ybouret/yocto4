#ifndef YOCTO_MATH_CHOLESKY_INCLUDED
#define YOCTO_MATH_CHOLESKY_INCLUDED 1

#include "yocto/math/matrix.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto 
{
	namespace math
	{
		
		template <typename T>
		struct cholesky
		{
			//! a.is_square() and diag.size() == a.rows
			/**
			 only the upper part of a is required, and
			 the result is stored in the lower triangle,
			 but the diagonal into diag.
			 */
			static bool build( matrix<T> &a, array<T> &diag ) throw();
			
			//! after cholesky::build(a,diag), b[1..n] and x[1..n], may be the same
			static void solve( const matrix<T> &a, const array<T> &diag, const array<T> &b, array<T> &x ) throw();
            
            //! a = J.J', symmetric
            static void Gram( matrix<T> &a, const matrix<T> &J );
            
            //! a.is_square() and diag.size() == a.rows
			/**
			 only the upper part of a is required, and
			 the result is stored in the lower triangle,
			 but the inverse diagonal into invdiag.
			 */
			static bool inv_build( matrix<T> &a, array<T> &inv_diag ) throw();
            
            //! after cholesky::inv_build(a,inv_diag), b[1..n] and x[1..n], may be the same
			static void inv_solve( const matrix<T> &a, const array<T> &inv_diag, const array<T> &b, array<T> &x ) throw();
		};
		
		
		
	}
	
}

#endif
