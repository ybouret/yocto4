#ifndef YOCTO_THREADING_THREAD_INCLUDED
#define YOCTO_THREADING_THREAD_INCLUDED 1

#include "yocto/threading/mutex.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
	
	namespace threading
	{
        
		class thread
		{
		public:
#if defined(YOCTO_BSD)
			typedef pthread_t handle_t;
			typedef pthread_t id_t;
#endif
			
#if defined(YOCTO_WIN)
			typedef HANDLE handle_t;
			typedef DWORD  id_t;
#endif
			
			typedef void (*proc_t)( void *);
			
			explicit thread( thread::proc_t proc, void *data);
			void     join()   throw();
			virtual ~thread() throw(); //!< must be joined before !
			
			id_t     get_id() const throw();
			handle_t get_handle() const throw();
            
			static id_t     get_current_id() throw();
			static handle_t get_current_handle() throw();
            
            void on_cpu( size_t cpu_id );
            
            static void assign_cpu( thread::handle_t , size_t cpu_id );
            static void foreach_cpu( thread::handle_t, void (*proc)(size_t cpu_id,void*), void *);
            
            
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(thread);
			proc_t       proc_;
			void        *data_;
#if defined(YOCTO_WIN)
			id_t         id32_;
#endif
			handle_t     handle_;
            
#if defined(YOCTO_BSD)
			static void * launch( void * ) throw();
#endif
			
#if defined(YOCTO_WIN)
			static DWORD WINAPI launch( LPVOID ) throw();
#endif
           
		};
     
    }
    
    
    //! for cheap threading
    class thread_proxy
    {
    public:
        explicit thread_proxy() throw();
        virtual ~thread_proxy() throw();
        
        void launch( threading::thread::proc_t proc, void *data );
        void finish() throw();
        
        template <typename FUNC>
        void launch( FUNC &fn )
        {
            launch( thread_proxy::execute<FUNC>, (void *)&fn );
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(thread_proxy);
        uint64_t block[ YOCTO_U64_FOR_ITEM(threading::thread) ];
        void clear() throw();
        template <typename FUNC> static inline
        void   execute( void *args ) throw()
        {
            assert(args);
            union
            {
                void *args;
                FUNC *func;
            } alias = { args };
            assert(alias.func);
            FUNC &fn = * alias.func;
            fn();
        }
    };
	
}

#endif
