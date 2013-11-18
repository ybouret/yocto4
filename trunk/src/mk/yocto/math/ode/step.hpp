#ifndef YOCTO_ODE_STEP_INCLUDED
#define YOCTO_ODE_STEP_INCLUDED 1


#include "yocto/math/ode/types.hpp"
#include "yocto/sequence/lw-arrays.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace ode
		{
			
			template <typename T>
			class step : public lw_arrays<T,memory_type>
			{
			public:
				typedef typename field<T>::type equation;
				
				virtual ~step() throw();
				
				virtual  void operator()(array<T>        &yout, //-- output
										 array<T>        &yerr, //-- embeded error
										 equation        &drvs, //-- equation
										 const T          x,    //-- starting point
										 const T          h,    //-- required step
										 const array<T>  &y,    //-- initial value      @x
										 const array<T>  &dydx  //-- initial derivative @x
										 ) = 0;
				
				
			protected:
				explicit step( const size_t num );
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(step);
			};
			
		}
		
	}
	
}

#endif
