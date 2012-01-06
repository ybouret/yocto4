//! \file functor.h
/**
 \brief Functor implementation
 */

#ifndef YOCTO_FUNCTOR_INCLUDED
#define YOCTO_FUNCTOR_INCLUDED 1


#include "yocto/core/functor-callback.hpp"
#include "yocto/core/functor-command.hpp"

namespace yocto
{
	
	
	
	//! functor
	/**
	 - R is the return type
	 - TLIST is the parameters type list
	 */
	template <typename R, class TLIST>
	class functor : public core::callable<R,TLIST>
	{
	public:
		typedef typename core::callable<R,TLIST>  callable;
		virtual callable  *clone() const { return new functor( *this); }
		
		//! destroy the internal wrappers
		inline virtual ~functor() throw() { assert(function_); destroy(); }
		
		
		//! copy: duplicate the functor context
		inline functor( const functor &other ) : function_( other.function_->clone() ) { assert( function_ ); }
		
		//! assign: duplicate source context
		inline functor & operator=( const functor &other )
		{
			if( this != &other )
			{
				//-- unsafe --
				callable *fn = other.function_->clone(); assert(fn);
				//--  safe  --
				destroy();
				function_ = fn;
			}
			return *this;
		}
		
		//! functionoid constructor
		/**
		 \param functionoid a callable object
		 functionoid can be of TWO types
		 - it can be a regular C function, whose arguments and return types are compatible
		 with the functor prototype.
		 - it can be a full C++ object with an operator()(...)  whose arguments and return types are compatible
		 with the functor prototype. In that case, THE C++ OBJECT IS COPIED, i.e. the whole
		 context is wrapped in that functor.
		 
		 This is achieved through the core::callback which uses the copy semantic of FUNCTION.
		 */
		template <typename FUNCTION>
		inline functor( const FUNCTION &functionoid ) :
		function_( new core::callback<R,TLIST,FUNCTION>(functionoid) ) {}
		
		//! object+method constructor
		/**
		 \param o an object pointer
		 \param m an object method, like &OBJECT_POINTER::DoSomething(...)
		 Since only the pointer is copied, this strategy can be used
		 for building one functor whose context can be change by tweaking the
		 original object.
		 */
		template <typename OBJECT_POINTER, typename METHOD_POINTER>
		inline functor( OBJECT_POINTER o, METHOD_POINTER m ) :
		function_( new core::command<R,TLIST,OBJECT_POINTER,METHOD_POINTER>( o, m ) ) {}
		
		//======================================================================
		// inner definitions
		//======================================================================
		typedef R        result;
		typedef TLIST    arguments;
		YOCTO_FUNCTOR_PARAMETERS();
		
		
		//======================================================================
		// forwarding calls, using optimized parameter types
		// to avoid copy overhead
		//======================================================================
		inline R operator()(void) {
			assert(function_);
			return (*function_)();
		}
		
		inline R operator()(param1 p1)
		{
			assert(function_);
			return (*function_)(p1);
		}
		
		inline R operator()(param1 p1,
							param2 p2)
		{
			assert( function_ );
			return (*function_)(p1,p2);
		}
		
		inline R operator()(param1 p1,
							param2 p2,
							param3 p3)
		{
			assert( function_ );
			return (*function_)(p1,p2,p3);
		}
		
		inline R operator()(param1 p1,
		                    param2 p2,
		                    param3 p3,
		                    param4 p4)
		{
			assert( function_ );
			return (*function_)(p1,p2,p3,p4);
		}
		
		//! direct assignation for binder_first
		explicit functor( callable *proc ) throw() : function_( proc ) { assert( proc ); }
		
	private:
		callable  *function_;
		inline void destroy() throw()
		{
			object *pObj = function_; // __DMC__ bug otherwise
			function_    = NULL;
			delete  pObj;
		}
	};
	
	
	namespace core {
		
		//! first parameter binding
		/**
		 inner typedefs perform all the types computations
		 */
		template <typename incoming>
		class binder_first : public callable<typename incoming::result, typename incoming::arguments::tail>
		{
		public:
			typedef typename incoming::result                     R;
			typedef typename incoming::arguments::tail            TLIST;
			typedef functor<R,TLIST>                              outgoing;
			
			typedef typename incoming::arguments::head            bounded;
			typedef typename type_traits<bounded>::parameter_type bounded_param;
			
			
			virtual callable<R,TLIST> *clone() const { return new binder_first( *this ); }
			
			explicit binder_first( const incoming &f, bounded_param b ) :
			functor_( f ),
			bounded_( b )
			{}
			
			virtual ~binder_first() throw() {}
			
			
			YOCTO_FUNCTOR_PARAMETERS();
			
			inline R operator()(void)
			{
				return functor_(bounded_);
			}
			
			inline R operator()(param1 p1)
			{
				return functor_(bounded_,p1);
			}
			
			
			
		private:
			incoming functor_;   //!< functor copy semantics
			bounded  bounded_;   //!< first parameter value
			
			binder_first( const binder_first &other ) :
			functor_( other.functor_ ),
			bounded_( other.bounded_ )
			{
			}
		};
	}
	
	template <typename incoming>
	inline
	typename core::binder_first<incoming>::outgoing
	bind_first( const incoming &f, typename core::binder_first<incoming>::bounded_param p )
	{
		typedef typename core::binder_first<incoming>::outgoing out_functor;
		typedef typename out_functor::callable                  out_callable;
		out_callable *proc = new core::binder_first<incoming>( f, p );
		return  out_functor( proc );
	}
	
	//! make a functor from a C function
	template <typename R,typename T>
	inline functor<R,TL1(T)> cfunctor( R (*cfn)(T) ) { return functor<R,TL1(T)>( cfn ); }
	
	//! make a functor from a C function
	template <typename R,typename T,typename U>
	inline functor<R,TL2(T,U)> cfunctor2( R (*cfn)(T,U) ) { return functor<R,TL2(T,U)>( cfn ); }
	
	//! make a functor from a C function
	template <typename R,typename T,typename U,typename V>
	inline functor<R,TL3(T,U,V)> cfunctor3( R (*cfn)(T,U,V) ) { return functor<R,TL3(T,U,V)>( cfn ); }
	
}

#endif
