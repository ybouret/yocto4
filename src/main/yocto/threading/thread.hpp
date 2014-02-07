#ifndef YOCTO_THREADING_THREAD_INCLUDED
#define YOCTO_THREADING_THREAD_INCLUDED 1

#include "yocto/threading/mutex.hpp"
#include "yocto/container/vslot.hpp"

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
            
            //! wrapper
            /**
             \warning fn must be available until finish
             */
            template <typename FUNC>
            inline void start( FUNC &fn )
            {
                args[0] = (void*)&fn;
                launch( execute<FUNC>, this);
            }
            
            //! wrapper
            /**
             \warning fn and x must be available until finish
             */
            template <typename FUNC, typename T>
            inline void start( FUNC &fn, T &x)
            {
                args[0] = (void*)&fn;
                args[1] = (void*)&x;
                launch( execute2<FUNC,T>, this);
            }
            
            
            handle_t        get_handle() const throw();
            static handle_t get_current_handle() throw();
            
            void on_cpu( size_t cpu_id );
            static void assign_cpu( thread::handle_t , size_t cpu_id );
            static void foreach_cpu( thread::handle_t, void (*proc)(size_t cpu_id,void*), void *);
            
        private:
            thread( mutex &shared_access ) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(thread);
            ~thread() throw();
            handle_t handle;
            proc_t   proc;    //!< routine to start
            void    *data;    //!< arguments for the routine
            void    *args[2]; //!< arguments for wrapper
            
            void     clear() throw();
            
            template <typename FUNC> static inline
            void   execute( void *args ) throw()
            {
                try
                {
                    assert(args!=0);
                    thread &self = *static_cast<thread *>(args);
                    assert( self.args[0] );
                    union
                    {
                        void *item;
                        FUNC *func;
                    } alias = { self.args[0] };
                    assert(alias.func);
                    FUNC &fn = * alias.func;
                    fn();
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
                    assert( self.args[0] );
                    assert( self.args[1] );
                    union
                    {
                        void *item;
                        FUNC *func;
                    } alias = { self.args[0] };
                    assert(alias.func);
                    FUNC &fn = * alias.func;
                    T    &x  = *(T *) self.args[1];
                    fn(x);
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
