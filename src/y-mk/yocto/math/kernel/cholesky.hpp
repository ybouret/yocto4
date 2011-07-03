#ifndef YOCTO_CHOLESKY_INCLUDED
#define YOCTO_CHOLESKY_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
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
			static bool apply( matrix<T> &a, array<T> &diag ) throw();
			
			//! after cholesky::apply(a,diag), b[1..n] and x[1..n], may be the same
			static void solve( const matrix<T> &a, const array<T> &diag, const T *b, T *x ) throw(); 
		};
		
		template <typename T, typename BVEC, typename XVEC>
		inline void cholesky_solve( const matrix<T> &a, const array<T> &diag, const BVEC &b, XVEC &x ) throw()
		{
			assert( b.size() == x.size() );
			assert( a.rows   == b.size() );
			assert( diag.size() == a.rows );
			cholesky<T>:: solve( a, diag, b(-1), x(-1) );
		}
		
	}
	
}

#endif
