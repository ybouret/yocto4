#ifndef YOCTO_THREADING_THREAD_INCLUDED
#define YOCTO_THREADING_THREAD_INCLUDED 1

#include "yocto/threading/mutex.hpp"
#include "yocto/container/vslot.hpp"

namespace yocto
{

	namespace threading
	{


		class threads;

		//! a generic thread
		class thread
		{
		public:
			//! the default prototype
			typedef void (*proc_t)(void *);

#if defined(YOCTO_BSD)
			//! pthread
			typedef pthread_t handle_t;
#endif

#if defined(YOCTO_WIN)
			//! win32
			typedef HANDLE handle_t;
#endif

			//! shared access
			mutex  &access;

			//! get memory and link access
			static thread *create_with( mutex & shared_access );

			//! finish MUST be called before !
			static void    destruct( thread *thr ) throw();

			//! launch a thread
			/**
			the stop flag is set to false.
			\warning the user_proc/user_data must be available until finish!
			*/
			void launch( proc_t user_proc, void *user_data );


			//! help for thread to finish
			/**
			if the thread is active,
			the stop flag is set to true and
			the shutdown procedure called.
			*/
			void finish() throw();

			//! get system level handle
			handle_t        get_handle() const throw();

			//! get system level handle without the wrapper
			static handle_t get_current_handle() throw();

			void on_cpu( size_t cpu_id );
			static void assign_cpu( thread::handle_t , size_t cpu_id );
			static void foreach_cpu( thread::handle_t, void (*proc)(size_t cpu_id,void*), void *);



		private:
			thread( mutex &shared_access ) throw();
			YOCTO_DISABLE_COPY_AND_ASSIGN(thread);
			~thread() throw();
			friend class threads;

			handle_t handle;
			proc_t   proc;    //!< routine to start
			void    *data;    //!< arguments for the routine
			vslot    code;    //!< store executable code

			void     clear() throw();


#if defined(YOCTO_BSD)
			static void * entry( void * ) throw();
#endif

#if defined(YOCTO_WIN)
			static DWORD WINAPI entry( LPVOID ) throw();
#endif


		public:
			const bool stop; //!< internal flag
			thread    *next;
			thread    *prev;

		};

		void assign_current_thread_on( size_t cpu_id );



	}



}

#endif
