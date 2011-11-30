#ifndef YOCTO_MATH_KERNEL_ALGEBRA_INCLUDED
#define YOCTO_MATH_KERNEL_ALGEBRA_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"

namespace yocto
{
	namespace math
	{
		
		template <typename T>
		struct algebra
		{
			static void mul( array<T> &v, const matrix<T> &M, const array<T> &u ) throw();
			static void mul_trn( array<T> &v, const matrix<T> &M, const array<T> &u ) throw();
			static void mul( matrix<T> &M, const matrix<T> &lhs, const matrix<T> &rhs) throw();
			static void mul_ltrn( matrix<T> &M, const matrix<T> &lhs, const matrix<T> &rhs) throw();
			static void mul_rtrn( matrix<T> &M, const matrix<T> &lhs, const matrix<T> &rhs) throw();
			static void gram_schmidt( matrix<T> &U, const matrix<T> &V, bool normalize);
		};
		
	}
	
}

#endif