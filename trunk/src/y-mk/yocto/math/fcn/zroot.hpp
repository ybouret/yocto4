#ifndef YOCTO_ZROOT_INCLUDED
#define YOCTO_ZROOT_INCLUDED 1


#include "yocto/math/fcn/jacobian.hpp"
#include "yocto/math/kernel/linsys.hpp"

namespace yocto
{
	
	namespace math
	{
	
		template <typename T>
		class zroot
		{
		public:
			explicit zroot();
			virtual ~zroot() throw();
			
			T ftol; //! for algorithm convergence
			
			void acquire(size_t n);
			void release() throw();
			size_t size() const throw();
			
			bool operator()( typename numeric<T>::vector_field &F, array<T> &X, const T h, jacobian<T> &dFdX);
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(zroot);
			vector<T> xcur_;
			vector<T> fcur_;
			vector<T> step_;
			vector<T> xnew_;
			vector<T> fnew_;
			matrix<T> jac_;
			linsys<T> lss_;
			typename numeric<T>::vector_field *pF_;
			T          hF2_(T);
			typename numeric<T>::function lmin_;
		};
		
	}
	
}

#endif
