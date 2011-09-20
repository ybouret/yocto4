#ifndef YOCTO_MATH_FIT_LSF_INCLUDED
#define YOCTO_MATH_FIT_LSF_INCLUDED 1

#include "yocto/math/fit/sample.hpp"
#include "yocto/math/fcn/derivative.hpp"
#include "yocto/sequence/lw-arrays.hpp"
#include "yocto/math/kernel/matrix.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace fit
		{
			
			template <typename T>
			class lsf
			{
			public:
				typedef functor<T,TL2(T,const array<T>)> field;
				
				
				void operator()(sample<T>          &s,
								field              &f,
								array<T>           &aorg,
								const array<bool>  &used,
								array<T>           &aerr,
								T                   ftol);
				
				explicit lsf();
				virtual ~lsf() throw();
				
				
			private:
				typedef lw_arrays<T,memory::global> arrays;
				typedef lw_array<T> array_t;
				typedef typename numeric<T>::function function_t;
				
				sample<T>         *samp_;
				field             *func_;
				const array<bool> *used_;
				size_t             nvar_;
				size_t             ndat_;
				arrays             harr_;
				array_t           &dFda_;
				array_t           &beta_;
				array_t           &aorg_;
				array_t           &atry_;
				array_t           &step_;
				matrix<T>          alpha_;
				matrix<T>          curv_;
				T                  xi_;    //!< to compute gradient
				size_t             iA_;    //!< to compute gradient
				derivative<T>      drvs_;  //!< to compute gradient
				function_t         grad_;  //!< gradient function wrapper
				
				T          grad_fn(T );
				void       gradient(  T xi );
				T          initialize();   //!< compute beta, alpha and sample.z, return intial D
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(lsf);
			};
			
			
		}
		
	}
	
}

#endif
