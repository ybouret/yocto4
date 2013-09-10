//! \file

#ifndef YOCTO_RUNNABLE_INCLUDED
#define YOCTO_RUNNABLE_INCLUDED 1

#include "yocto/threading/thread.hpp"
#include "yocto/code/round.hpp"



#define	YOCTO_ARGUMENTS_DECL_SUBKEY                                              \
typedef SUBKEY                                      subkey_type;                 \
typedef typename type_traits<SUBKEY>::mutable_type  mutable_subkey;              \
typedef const mutable_subkey                        const_subkey;                \
typedef typename                                                                 \
yocto::select<                                                                   \
type_traits<mutable_subkey>::is_primitive, /* no references in containers */     \
const_subkey  , /* pass primitive mutable types by const value            */     \
const_subkey &  /* pass non primitive mutable types by const reference    */     \
>::result param_subkey


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
