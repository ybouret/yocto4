#ifndef YOCTO_THREADING_THREADS_INCLUDED
#define YOCTO_THREADING_THREADS_INCLUDED 1

#include "yocto/threading/thread.hpp"
#include "yocto/core/pool.hpp"
#include "yocto/core/list.hpp"
#include "yocto/auto-clean.hpp"

namespace yocto
{
    
    namespace threading
    {
     
        
        //! low level memory for threads
        class threads : public core::list_of<thread>
        {
        public:
            mutex access;
            
            //! construct and give name to access
            explicit threads(const char *id) throw();

            //! construct, give name to access and reserve(n)
            explicit threads(const char *id, size_t n) throw();

            //! finish and clean all
            virtual ~threads() throw();
            
            //! launch a generic procedure
            void launch( thread::proc_t proc, void *data);
            
            //! finish a specific thread, removed from the list
            void finish( thread *thr ) throw();
            
            //! finish all
            void finish() throw();
            
            //! release pool
            void trim() throw();
            
            //! wrapper
            template <typename FUNC>
            inline void start( FUNC &fn )
            {
                thread *thr = query();
                try       { thr->start<FUNC>(fn); push_back(thr); }
                catch(...){ pool.store(thr); throw; }
            }
            
            //! wrapper
            template <typename FUNC, typename T>
            inline void start( FUNC &fn, T &x )
            {
                thread *thr = query();
                try       { thr->start<FUNC,T>(fn,x); push_back(thr); }
                catch(...){ pool.store(thr); throw; }
            }

            
            template <typename OBJECT_POINTER, typename OBJECT_METHOD>
            inline void call( OBJECT_POINTER pObj, OBJECT_METHOD meth )
            {
                thread *thr = query();
                try       { thr->call<OBJECT_POINTER,OBJECT_METHOD>(pObj,meth); push_back(thr); }
                catch(...){ pool.store(thr); throw; }

            }
            
            //! prepare some memory
            void reserve(size_t n);
            
            
            class failsafe : public auto_clean<threads>
            {
            public:
                explicit failsafe( threads &host ) throw();
                virtual ~failsafe() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(failsafe);
            };
        private:
            thread *query();
            core::pool_of<thread> pool;
            YOCTO_DISABLE_COPY_AND_ASSIGN(threads);
        };
        
    }
    
}


#endif
