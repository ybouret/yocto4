#include "yocto/threading/server.hpp"

namespace yocto
{
    namespace threading
    {
        
        server::task:: ~task() throw() {}
        
        server::task:: task( const server::job &J ) :
        next(0),
        prev(0),
        work(J)
        {
            
        }
        
        
        server:: ~server() throw()
        {
            
        }
        
        
        server:: server() :
        workers( "server" ),
        access( workers.access ),
        tasks(),
        tpool()
        {
            
        }
        
        void server:: enqueue( const job &J )
        {
            YOCTO_LOCK(access);
            task *t = tpool.size ? tpool.query() : object::acquire1<task>();
            try
            {
                //______________________________________________________________
                //
                // create the new task
                //______________________________________________________________
                new (t) task(J);
                tasks.push_front(t);
               
                //______________________________________________________________
                //
                // try to start it
                //______________________________________________________________

                
            }
            catch(...)
            {
                tpool.store(t);
                throw;
            }
            
        }

        
    }

}
