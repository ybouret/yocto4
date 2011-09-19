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
								const array<bool>  &used);
				
				explicit lsf();
				virtual ~lsf() throw();
				
				
			private:
				typedef lw_arrays<T,memory::global> arrays;
				typedef lw_array<T> array_t;
				size_t             nvar_;
				size_t             ndat_;
				arrays             harr_;
				array_t           &dFda_;
				array_t           &beta_;
				array_t           &aorg_;
				array_t           &atry_;
				matrix<T>          alpha_;
				derivative<T>      drvs_; //!< to compute gradient
				void       gradient( field &func, T xi, array<T> &a, const array<bool> &used );
				void       initialize( sample<T> &s, field &f, const array<bool> &used);
				
				YOCTO_DISABLE_COPY_AND_ASSIGN(lsf);
			};
			
			
		}
		
	}
	
}

#endif
