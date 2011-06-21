#ifndef YOCTO_THREADING_CREW_INCLUDED
#define YOCTO_THREADING_CREW_INCLUDED 1

#include "yocto/threading/barrier.hpp"
#include "yocto/threading/condition.hpp"

#include "yocto/memory/buffers.hpp"
#include "yocto/threading/thread.hpp"

//#include <iostream>

namespace yocto
{
	
	namespace threading
	{
		//======================================================================
		//! light-weight static SIMD crew
		//
		//
		//======================================================================
		class crew : public object
		{
		public:
			class context
			{
			public:
				const size_t rank;
				const size_t size;
				mutex       &critical;
				context( size_t , size_t , mutex & ) throw();
				~context() throw();
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(context);
			};
			
			class worker
			{
			private:
				thread  thread_;
			public:
				const size_t rank;  //!< 0..size-1
				void        *args;  //!< for crew.proc
			private:
				crew        *crew_;				
				worker();    ~worker();
				YOCTO_DISABLE_COPY_AND_ASSIGN(worker);
				friend class crew;
			};
			
			
			explicit crew(size_t n);
			virtual ~crew() throw();
			void run() throw();
			
			const size_t size; 
			void       (*proc)( context &, void * ) throw();
			
			
			template <typename ITER>
			void set_args( ITER iter ) throw()
			{
				for( size_t i=0; i < size; ++i, ++iter )
				{
					agent_[i].args = & (*iter);
					//std::cerr << "args[" << i << "]=" << agent_[i].args << std::endl;
				}
			}
			
			void set_all_args( void *args ) throw();
			
			
			worker       & operator[]( size_t r ) throw(); //0..size-1
			const worker & operator[]( size_t r ) const throw(); //0..size-1
			
			mutex & critical() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(crew);
			mutex     guard_;  //!< shared mutex
			condition start_;  //!< threads are waiting or work
			condition cycle_;  //!< application is waiting for threads
			barrier   fence_;  //!< end of cycle synchronization, last thread cycle_.broadcast()
			bool      valid_;  //!< to signal thread termination
			worker   *agent_;  //!< size agents
			size_t    count_;  //!< successfully created
			size_t    ready_;  //!< shared ready
			
			void terminate()   throw();
			void check_ready() throw();
			
			static void engine( void * ) throw();
		};
		
		
	}
	
}

#endif
