#ifndef YOCTO_THREADING_SIMD_INCLUDED
#define YOCTO_THREADING_SIMD_INCLUDED 1


#include "yocto/threading/condition.hpp"

namespace yocto
{
	namespace threading
	{
		
		class SIMD
		{
		public:
			explicit SIMD( size_t np );
			virtual ~SIMD() throw();
			
			void cycle() throw();
			
			const size_t threads;
		private:
			bool      _stop_;
			mutex     guard_;   //!< shared mutex
			condition start_;   //!< constructed/ready to start condition
			condition final_;   //!< final condition (no more working threads)
			size_t    ready_;   //!< used for #ready  threads
			size_t    active_;  //!< used for #active threads 
			size_t    wlen_;    //!< bytes for workspace
			void     *wksp_;    //!< workspace for threads/data
			size_t    counter_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(SIMD);
			void terminate() throw();
			static void CEngine(void*) throw();
			void engine(size_t rank) throw();
			void check_ready() throw();
		};
		
	}
	
}


#endif
