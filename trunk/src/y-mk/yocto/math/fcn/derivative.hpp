#ifndef YOCTO_DERIVATIVE_INCLUDED
#define YOCTO_DERIVATIVE_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/c-array.hpp"

namespace yocto
{
	namespace math
	{
		
		template <typename T>
		class derivative
		{
		public:
			static const size_t kmax_ = 8;
			static const T      hdiv_;
			
			explicit derivative() throw();
			virtual ~derivative() throw();
			
			T operator()( typename numeric<T>::function &, T x, T h);
			
			void gradient( array<T>  &grad, typename numeric<T>::scalar_field &F, const array<T> &x, T h );				
			
			
		private:
			typedef c_array<T> array_t;
			array_t x_; //!< reduced step
			array_t y_; //!< extrapolation
			array_t c_; //!< aux. array
			array_t d_; //!< aux. array
			T wksp_[ kmax_ * 4]; //!< workspace
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(derivative);
		};
		
	}
	
}

#endif
