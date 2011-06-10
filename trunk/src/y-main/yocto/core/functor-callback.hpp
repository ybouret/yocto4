#ifndef YOCTO_FUNCTOR_CALLBACK_INCLUDED
#define YOCTO_FUNCTOR_CALLBACK_INCLUDED 1

#include "yocto/core/functor-call.hpp"

namespace yocto {

	namespace core
	{

		//! wrapper for a callable entity
		/**
		 * this can be a C function or a functionoid
		 *
		 */
		template <typename R, typename TLIST, typename FUNCTION>
		class callback : public virtual callable<R,TLIST>
		{
		public:
			explicit callback( const FUNCTION &fn ) : function_( fn ) {}
			virtual ~callback() throw()	{}
			virtual callable<R,TLIST> *clone() const { return new callback( *this ); }

			YOCTO_FUNCTOR_PARAMETERS();

			inline R operator()(void)
			{
				return function_();
			}

			inline R operator()( param1 P1 )
			{
				return function_( P1 );
			}

			inline R operator()( param1 P1, param2 P2 )
			{
				return function_( P1, P2 );
			}

			inline R operator()( param1 P1, param2 P2, param3 P3)
			{
				return function_( P1, P2, P3 );
			}

			inline R operator()( param1 P1, param2 P2, param3 P3, param4 P4)
			{
				return function_( P1, P2, P3, P4);
			}


		private:
			YOCTO_DISABLE_ASSIGN(callback);
			//! use the copy semantics of FUNCTION to clone
			explicit callback( const callback &other ) : function_( other.function_ ) {}
			FUNCTION function_;
		};

	}

}

#endif
