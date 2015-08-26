#include "yocto/threading/threads.hpp"

namespace yocto
{
    namespace threading
    {
        threads:: threads(const char *id) throw() :
        core::list_of<thread>(),
        access(id),
        pool()
        {
            
        }
        
        threads:: threads(const char *id, size_t n) throw() :
        core::list_of<thread>(),
        access(id),
        pool()
        {
            reserve(n);
        }
        
        threads:: ~threads() throw()
        {
            finish();
            trim();
        }
        
        
        void threads:: finish( thread *thr ) throw()
        {
            assert(thr);
            assert(this->owns(thr));
            thr->finish();
            pool.store( unlink(thr) );
        }
        
        
        void threads:: finish() throw()
        {
            while(size)
            {
                thread *thr = pop_back();
                thr->finish();
                pool.store(thr);
            }
        }
        
        void threads:: trim() throw()
        {
            while( pool.size ) thread::destruct( pool.query() );
        }
        
        thread * threads:: query()
        {
            return pool.size ? pool.query() : thread::create_with(access);
        }
        
        void threads:: launch( thread::procedure proc, void *data)
        {
            YOCTO_LOCK(access);
            thread *thr = query();
            try
            {
                thr->launch(proc, data);
                push_back(thr);
            }
            catch(...)
            {
                pool.store(thr);
                throw;
            }
        }
        
        
        void threads:: reserve(size_t n)
        {
			YOCTO_LOCK(access);
            while(n-->0) pool.store( thread::create_with(access) );
        }
        
        threads:: failsafe:: failsafe( threads &host ) throw() :
        auto_clean<threads>(host, & threads::finish )
        {
            
        }
        
        threads:: failsafe:: ~failsafe() throw()
        {
        }
        
    }
}
