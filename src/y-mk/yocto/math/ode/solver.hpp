#ifndef YOCTO_MATH_ODE_SOLVER_INCLUDED
#define YOCTO_MATH_ODE_SOLVER_INCLUDED 1

#include "yocto/math/ode/control.hpp"

namespace yocto
{
	
	namespace math
	{
		
		namespace ode
		{
			
			template <typename T>
			class solver : public lw_arrays<T,memory_type>
			{
			public:
				typedef typename field<T>::type equation;
				explicit solver();
				virtual ~solver() throw();
				
				void operator()(equation   &drvs,
								control<T> &ctrl,
								step<T>    &forward,
								array<T>   &ystart,
								const T   x1,
								const T   x2,
								T        &h1
								);
				
				T hmin; //!< initial=0
				T eps;  //!< initial=ftol
				T TINY; //!< initial=1e-30, for yscal
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(solver);
				lw_array<T> &y, &dydx, &yscal;
			};
			
			
			template <
			typename T,
			template <typename> class CTRL,
			template <typename> class STEP
			> class driver : public solver<T>
			{
			public:
				typedef typename field<T>::type equation;
				
				explicit driver() : solver<T>(), ctrl_(), step_() {}
				virtual ~driver() throw()       {}
				
				inline void start( size_t nvar ) 
				{ 
					this->prepare( nvar );
					ctrl_.prepare( nvar );
					step_.prepare( nvar );
				}
				
				inline void operator()(equation   &drvs,
									   array<T>   &ystart,
									   const T     x1,
									   const T     x2,
									   T          &h1
									   )
				{
					solver<T> &self = *this;
					self( drvs, ctrl_, step_, ystart, x1, x2, h1 );
				}
				
			private:
				CTRL<T> ctrl_;
				STEP<T> step_;
			};
			
		}
		
	}
	
}

#endif
