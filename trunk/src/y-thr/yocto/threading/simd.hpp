#ifndef YOCTO_THREADING_SIMD_INCLUDED
#define YOCTO_THREADING_SIMD_INCLUDED 1


#include "yocto/threading/condition.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	namespace threading
	{
		
		class SIMD
		{
		public:
			
			class Context
			{
			public:
				Context( const size_t thread_id, const size_t num_threads ) throw() :
				rank( thread_id   ),
				size( num_threads )
				{}
				
				~Context() throw() {}
				const size_t      rank;   //!< in 0..size-1
				const size_t      size;   //!< num threads
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Context);
			};
			
			typedef  const SIMD::Context &    Args;
			typedef  functor<void,TL1(Args)>  Proc;
			explicit SIMD( size_t np );
			virtual ~SIMD() throw();
			
			void cycle( Proc &proc ) throw();
			
			const size_t threads;
		private:
			bool      _stop_;
			mutex     guard_;   //!< shared mutex
			condition start_;   //!< constructed/ready to start condition
			condition final_;   //!< final condition (no more working threads)
			size_t    ready_;   //!< used for #ready  threads
			size_t    active_;  //!< used for #active threads
			Proc     *proc_;    //!< what to do during one cycle
		public:
			Proc      idle;    //!< do nothing proc
		private:
			size_t    wlen_;    //!< bytes for workspace
			void     *wksp_;    //!< workspace for threads/data
			size_t    counter_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(SIMD);
			void terminate() throw();
			static void CEngine(void*) throw();
			void engine(size_t rank) throw();
			void check_ready() throw();
			void idle_( Args ) throw();
		};
		
	}
	
}


#endif
