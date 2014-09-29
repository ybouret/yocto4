/** \file extrapolate.hpp
*/

#ifndef _YOCTO_EXTRAPOLATE_HPP_
#define _YOCTO_EXTRAPOLATE_HPP_ 1

#include "yocto/math/kernel/matrix.hpp"

namespace yocto {

	namespace math {

		template <typename T>
		struct extrapolate 
		{
			//! extrapolation at zero.
			/**
				extrapolate at x=0 the function tabulated by
				\f$ xa[1]>xa[2]>..>xa[xa.size()] \f$.
				\param n  n>0, number of points to use
				\param xa xa.size() >= n
				\param ya ya.size() == xa.size().
				\param dy modified error estimate.
				\param c  c.size() == xa.size(), auxiliary vector.
				\param d  d.size() == xa.size(), auxiliary vector.

				\return the extrapolation value.
			*/
			static T get( size_t n, const array<T> &xa, const array<T> &ya, T &dy, array<T> &c, array<T> &d ) throw();

			//! multiple  extrapolations at zero.
			/**
				\param yxtr 1..nvar,   results.
				\param yerr 1..nvar,   error estimates.
				\param xa   1..nmax,   tabulated x values.
				\param ya   1..nmax:1..nvar, tabulated y values, each row is a y array !
				\param n    1<=n<=nmax,  order to perform extrapolation.
				\param c    1..nmax:1..nvar, auxiliary matrix.
				\param d    1..nmax:1..nvar, auxiliary matrix.
			*/
			static void get( array<T> &yxtr, array<T> &yerr, const array<T> &xa, const matrix<T> &ya, size_t n, matrix<T> &c, matrix<T> &d ) throw();
            
            //! using Pade approximant
            static T zpade( const array<T> &xa, const array<T> &ya );
            
		};

	} // math

} //yocto
#endif
