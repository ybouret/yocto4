#include "yocto/mpi/async.hpp"
#include <cstring>

namespace yocto
{
    
    void mpi_async:: clear() throw()
    {
    }
    
    mpi_async:: ~mpi_async() throw()
    {
        stop();
    }
    
    
    void mpi_async:: lock_on_ready() throw()
    {
        for(;;)
        {
            if( access.try_lock() )
            {
                if(ready)
                    break;
                access.unlock();
            }
        }
    }
    
    void mpi_async:: load_requests(mpi::Requests *req) throw()
    {
        ready    = false;
        requests = req;
    }
    
    void mpi_async:: launch( mpi::Requests &req )
    {
        
        lock_on_ready();
        load_requests(&req);
        resume_thread();
        
    }
    
    void mpi_async:: resume_thread() throw()
    {
        enter.signal();
        access.unlock();
    }
    
    
    void mpi_async:: stop() throw()
    {
        
        lock_on_ready();
        load_requests(0);
        resume_thread();
        
        //----------------------------------------------------------------------
        // final cleanup
        //----------------------------------------------------------------------
        workers.finish();
    }
    
    void mpi_async:: finish() throw()
    {
        leave.wait();
        
    }
    
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
    mpi_async:: mpi_async( const mpi & ref ) :
    MPI( ref ),
    ready(false),
    requests(0),
    workers("MPI.Async"),
    access(workers.access),
    enter(),
    leave(2,"MPI.Async.Barrier")
    {
        workers.launch( mpi_async::call,this);
    }
    
    void mpi_async:: call( void *args ) throw()
    {
        assert(args);
        static_cast<mpi_async *>(args)->loop();
    }
    
    void mpi_async:: loop() throw()
    {
        assert( ready == false   );
        assert( 0     == requests);
        
        try
        {
            
            { YOCTO_LOCK(access); MPI.Printf(stderr,"[Thread] Loop\n"); }
            
        CYCLE:
            //----------------------------------------------------------------------
            // acquire a lock
            //----------------------------------------------------------------------
            access.lock();
            
            //----------------------------------------------------------------------
            // allow main thread to process
            //----------------------------------------------------------------------
            ready = true;
            //MPI.Printf(stderr, "[Thread] Ready\n");
            //----------------------------------------------------------------------
            // Wait => unlock access
            //----------------------------------------------------------------------
            enter.wait(access);
            
            //----------------------------------------------------------------------
            // return on a locked mutex
            //----------------------------------------------------------------------
            assert(false==ready);
            //MPI.Printf( stderr, "[Thread] Run\n");
            access.unlock();
            
            if( requests )
            {
                MPI.Waitall( *requests );
                leave.wait();
                goto CYCLE;
            }
            //----------------------------------------------------------------------
            // at this point: Final
            //----------------------------------------------------------------------
            { YOCTO_LOCK(access); MPI.Printf(stderr,"[Thread] Done\n"); }
            
        }
        catch(...)
        {
            fprintf( stderr, "Something Went Wrong...\n");
        }
    }
    
}
