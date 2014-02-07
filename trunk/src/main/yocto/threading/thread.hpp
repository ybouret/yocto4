#ifndef YOCTO_THREADING_THREAD_INCLUDED
#define YOCTO_THREADING_THREAD_INCLUDED 1

#include "yocto/threading/mutex.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	
	namespace threading
	{
        
        
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
             \warning the user_proc must be available until finish!
             */
            void launch( proc_t user_proc, void *user_data );
            
            //! help for thread to finish
            /**
             if the thread is active,
             the stop flag is set to true and
             the shutdown procedure called.
             */
            void finish() throw();
            
            
            handle_t        get_handle() const throw();
            static handle_t get_current_handle() throw();
            
            void on_cpu( size_t cpu_id );
            static void assign_cpu( thread::handle_t , size_t cpu_id );
            static void foreach_cpu( thread::handle_t, void (*proc)(size_t cpu_id,void*), void *);
            
            
            template <typename FUNC,typename T>
            class call_start
            {
            public:
                FUNC *func;
                T    *args;
                inline  call_start( FUNC &f, T *x ) throw() : func(&f), args(x) {}
                inline ~call_start() throw() {}
                inline  call_start( FUNC &f ) throw() : func(&f), args(0) {}
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(call_start);
            };
            //! wrapper
            /**
             \warning fn must be available until finish
             */
            template <typename FUNC>
            inline void start( FUNC &f )
            {
                args.build< call_start<FUNC,void>, FUNC& >(f);
                launch( execute<FUNC>, this);
            }
            
            //! wrapper
            /**
             \warning fn and x must be available until finish
             */
            template <typename FUNC, typename T>
            inline void start( FUNC &f, T &x)
            {
                args.build< call_start<FUNC,T>, FUNC&, T* >(f,&x);
                launch( execute2<FUNC,T>, this);
            }
            
            
            typedef functor<void,null_type> callback;
            
            template <typename OBJECT_POINTER, typename OBJECT_METHOD>
            inline void call( OBJECT_POINTER pObj, OBJECT_METHOD meth )
            {
                args.build< callback, OBJECT_POINTER, OBJECT_METHOD >(pObj,meth);
                launch( executeCB, this);
            }
            
            
        private:
            thread( mutex &shared_access ) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(thread);
            ~thread() throw();
            handle_t handle;
            proc_t   proc;    //!< routine to start
            void    *data;    //!< arguments for the routine
            vslot    args;
            
            void     clear() throw();
            
            template <typename FUNC> static inline
            void   execute( void *args ) throw()
            {
                try
                {
                    assert(args!=0);
                    thread &self = *static_cast<thread *>(args);
                    call_start<FUNC,void> &call = self.args.as< call_start<FUNC,void> >();
                    assert(call.args==0);
                    (*call.func)();
                }
                catch(...)
                {
                }
            }
            
            template <typename FUNC,typename T> static inline
            void   execute2( void *args ) throw()
            {
                try
                {
                    assert(args);
                    thread &self = *static_cast<thread *>(args);
                    call_start<FUNC,T> &call = self.args.as< call_start<FUNC,T> >();
                    assert(call.args!=0);
                    (*call.func)( * call.args );
                }
                catch(...)
                {
                }
            }
            
            static inline void executeCB(void *args) throw()
            {
                try
                {
                    assert(args);
                    thread &self = *static_cast<thread *>(args);
                    self.args.as<callback>()();
                }
                catch(...)
                {
                }
            }
            
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
