#ifndef YOCTO_MATH_ODE_EXPLICIT_SOLVER_INCLUDED
#define YOCTO_MATH_ODE_EXPLICIT_SOLVER_INCLUDED 1

#include "yocto/math/ode/explicit/controler.hpp"
#include "yocto/math/ode/solver-data.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            //! explicit solver
            /**
             needs to be prepared before integration.
             */
            template <typename T>
			class explicit_solver : public solver_data<T>
			{
			public:
				typedef typename Field<T>::Equation equation;
                typedef typename Field<T>::Callback callback;
                
				explicit explicit_solver( T user_eps );
				virtual ~explicit_solver() throw();
				
                
                //! ystart@x1 -> x1
                /**
                 \param drvs    differential equation
                 \param ctrl    an adaptive controler
                 \param forward an embedded step
                 \param ystart  initial values
                 \param x1      initial parameter
                 \param x2      final   parameter
                 \param h1      guess step, updated
                 */
				void operator()(equation              &drvs,
								explicit_controler<T> &ctrl,
								explicit_step<T>      &forward,
								array<T>              &ystart,
								const T                x1,
								const T                x2,
								T                     &h1,
                                callback              *cb
								);
				
				
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(explicit_solver);
			};
            
            
            //! parametrized driver
            template <
			typename T,
			template <typename> class CTRL,
			template <typename> class STEP
			> class explicit_driver : public explicit_solver<T>
			{
			public:
				typedef typename Field<T>::Equation equation;
                typedef typename Field<T>::Callback callback;

				explicit explicit_driver( T user_eps ) : explicit_solver<T>( user_eps ), ctrl_(), step_() {}
				virtual ~explicit_driver() throw() {}
				
                //! allocate all memory
				inline void start( size_t nvar )
				{
					this->allocate( nvar );
					ctrl_.allocate( nvar );
					step_.allocate( nvar );
				}
				
                //! make a step
				inline void operator()(equation   &drvs,
									   array<T>   &ystart,
									   const T     x1,
									   const T     x2,
									   T          &h1,
                                       callback   *cb
									   )
				{
                    explicit_solver<T> &self = *this;
					self( drvs, ctrl_, step_, ystart, x1, x2, h1,cb);
				}
				
			private:
				CTRL<T> ctrl_;
				STEP<T> step_;
                YOCTO_DISABLE_COPY_AND_ASSIGN(explicit_driver);
			};
            
            
        }
    }
}


#endif
