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
            typedef void (*c_proc)(void);
            
            
            
            //! high level
            template <typename FUNCTION>
            class exec0
            {
            public:
                inline exec0( const FUNCTION &function ) : fn_(function)
                {
                }
                
                inline ~exec0() throw() {}
                
                inline void run()
                {
                    fn_();
                }
                
            private:
                FUNCTION fn_;
                YOCTO_DISABLE_COPY_AND_ASSIGN(exec0);
            };
            
            template <typename FUNCTION, typename T>
            class exec1
            {
            public:
                inline exec1(const FUNCTION                         &function,
                             typename type_traits<T>::parameter_type param1
                             ) :
                fn_(function),
                p1_(param1)
                {
                }
                
                inline ~exec1() throw() {}
                
                inline void run()
                {
                    fn_(p1_);
                }
                
            private:
                FUNCTION                              fn_;
                typename type_traits<T>::mutable_type p1_;
                YOCTO_DISABLE_COPY_AND_ASSIGN(exec1);
            };

            
            
            
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
            
            template <typename FUNCTION>
            void start( const FUNCTION &fn )
            {
                YOCTO_LOCK(access);
                assert(0==proc);
                assert(0==data);
                
                code.build< exec0<FUNCTION>, FUNCTION>( fn );
                
                launch( execute0<FUNCTION>, & code.as< exec0<FUNCTION> >() );
            }
            
            template <typename FUNCTION, typename T>
            void start( const FUNCTION &function, typename type_traits<T>::parameter_type param1 )
            {
                YOCTO_LOCK(access);
                assert(0==proc);
                assert(0==data);
            
                code.build< exec1<FUNCTION,T>, T>( function, param1);
                
                launch( execute1<FUNCTION,T>, & code.as< exec1<FUNCTION,T> >() );
            }
            
            
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
            
        private:
            template <typename FUNCTION>
            static inline
            void execute0( void *args )
            {
                try
                {
                    assert(args);
                    static_cast< exec0<FUNCTION> *>(args)->run();
                }
                catch(...)
                {
                    
                }
            }
            
            template <typename FUNCTION, typename T>
            static inline
            void execute1( void *args )
            {
                try
                {
                    assert(args);
                    static_cast< exec1<FUNCTION,T> *>(args)->run();
                }
                catch(...)
                {
                    
                }
            }

        };
        
        void assign_current_thread_on( size_t cpu_id );
        
        
        
    }
    
    
   	
}

#endif
