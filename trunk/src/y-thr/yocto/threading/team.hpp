//! \file 
#ifndef YOCTO_THREADING_TEAM_INCLUDED
#define YOCTO_THREADING_TEAM_INCLUDED 1


#include "yocto/threading/condition.hpp"
#include "yocto/threading/layout.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	namespace threading
	{
        
		//! easy multi threading, SIMD style
		class team : public layout
		{
		public:
			
			//! MPI style info for one thread
			class context
			{
			public:
				context(const size_t thread_id, 
                        const size_t num_threads, 
                        lockable    &guard ) throw();				
				~context() throw();
				const size_t      rank;   //!< in 0..size-1
                const size_t      indx;   //!< rank+1 in 1..size
				const size_t      size;   //!< number of thread in the team
				lockable         &access; //!< mutex
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(context);
			};
            
			//! what to do for each context
			typedef  functor<void,TL1(context&)> task;

            //! splitting prototype
            static void split( size_t &shift, size_t &count, size_t huge, size_t rank, size_t size) throw();
						
			//! build a team based on a parsed layout
			explicit team(const char *mutex_id = 0);
			
            //! build a team with manual settings
            explicit team( size_t num_threads, size_t thread_offset, const char *mutex_idx = 0);
            
			//! clean up
			virtual ~team() throw();
			
			//! perform task todo on each context
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
            void initialize();
            
			void terminate() throw();
			
			static void launcher(void*) throw();
			
			void engine(size_t rank) throw();
			void check_ready() throw();
			void place();
			
		};
		
	}
	
}


#endif
