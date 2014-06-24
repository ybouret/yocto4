#ifndef YOCTO_IOS_LOCAL_FILE_INCLUDED
#define YOCTO_IOS_LOCAL_FILE_INCLUDED 1


#include "yocto/threading/mutex.hpp"

namespace yocto
{

	namespace ios
	{

		//! local_file handling
		/**
			smart pointer to an internal mutex
		*/
		class local_file :  public lockable
		{
		public:
			enum type_t
			{
				is_regular,
				is_stdin,
				is_stdout,
				is_stderr,
                is_pipe
			};

			typedef int2type<is_stdin>  cstdin_t;
			typedef int2type<is_stdout> cstdout_t;
			typedef int2type<is_stderr> cstderr_t;

			virtual ~local_file() throw();
			explicit local_file( type_t t );			

			inline virtual void lock()     throw() { assert(access_); access_->lock();   }
			inline virtual void unlock()   throw() { assert(access_); access_->unlock(); }
			inline virtual bool try_lock() throw() { assert(access_); return access_->try_lock(); }

			const   type_t            type;
			
		private:
			mutable lockable         *access_;
			static  threading::mutex  stdio_lock;
			YOCTO_DISABLE_COPY_AND_ASSIGN(local_file);
		};

		extern const local_file::cstdin_t  cstdin;
		extern const local_file::cstdout_t cstdout;
		extern const local_file::cstderr_t cstderr;

	}
}


#endif
