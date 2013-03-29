//! \file

#ifndef YOCTO_RUNNABLE_INCLUDED
#define YOCTO_RUNNABLE_INCLUDED 1

#include "yocto/threading/thread.hpp"
#include "yocto/code/round.hpp"
#include "yocto/type-traits.hpp"

namespace yocto {
	
	
	namespace threading
	{
		//! base class for run() in a thread
		class runnable
        {
        public:
            virtual ~runnable() throw(); //!< join() MUST be called if start() was called
            void    start();             //!< call the run() command in one new thread
            void    join() throw();      //!< wait for run()  to complete
            
            
        protected:
            explicit runnable() throw();
            
        private:
            virtual void run() throw() = 0;
            YOCTO_DISABLE_COPY_AND_ASSIGN(runnable);
            uint64_t layout_[ YOCTO_U64_FOR_ITEM(thread) ];
            thread  *thread_;
            static void execute( void * ) throw();
            void        cleanup() throw();
        };
        
        template <
        typename HOST,
        typename T
        >
        class method_run : public runnable
        {
        public:
            
            explicit method_run( const HOST &host ) throw() :
            host_( host )
            {
            }
            
            virtual ~method_run() throw()
            {
            }
            
        private:
            HOST & host_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(method_run);
        };
        
	}
	
}


#endif
