#ifndef YOCTO_THREADING_TEAM_INCLUDED
#define YOCTO_THREADING_TEAM_INCLUDED 1


#include "yocto/threading/condition.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	namespace threading
	{
		class layout
		{
		public:
			const size_t size; //!< #threads requested
			const size_t root; //!< #CPU to start.
			explicit layout(); //!< parse the YOCTO_TEAM=#threads[,offset]"
			virtual ~layout() throw();
			layout( const layout &other ) throw();
			
		private:
			YOCTO_DISABLE_ASSIGN(layout);
		};
		
		class team : public layout
		{
		public:
			
			
			
			class context
			{
			public:
				context( const size_t thread_id, const size_t num_threads, lockable &guard ) throw();				
				~context() throw();
				const size_t      rank;   //!< in 0..size-1
				const size_t      size;   //!< num threads
				lockable         &access; //!< mutex
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(context);
			};
			
			typedef  functor<void,TL1(context&)> task;
			explicit team();
			virtual ~team() throw();
			
			void cycle( task &todo ) throw();
			
			
			
		private:
			bool      _stop_;
			mutex     guard_;   //!< shared mutex
			condition start_;   //!< constructed/ready to start condition
			condition final_;   //!< final condition (no more working threads)
			size_t    ready_;   //!< used for #ready  threads
			size_t    active_;  //!< used for #active threads
			task     *task_;    //!< what to do during one cycle
			size_t    wlen_;    //!< bytes for workspace
			void     *wksp_;    //!< workspace for threads/data
			size_t    counter_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(team);
			void terminate() throw();
			
			static void launcher(void*) throw();
			
			void engine(size_t rank) throw();
			void check_ready() throw();
			void place();
			
		};
		
	}
	
}


#endif
