#include "yocto/mpi/comm-thread.hpp"

#include <iostream>

namespace yocto
{
    
    mpi_comm_thread:: ~mpi_comm_thread() throw()
    {
        //stop();
    }
    
    mpi_comm_thread:: mpi_comm_thread( const mpi &ref ) :
    MPI(ref),
    access("MPI::Sync"),
    ready(false),
    enter(),
    thr( mpi_comm_thread::call, this)
    {
        
    }
    
    void mpi_comm_thread:: stop() throw()
    {
        _start(0);
        thr.join();
    }
    
    
    void  mpi_comm_thread:: call(void *args) throw()
    {
        assert(args);
        static_cast<mpi_comm_thread *>(args)->loop();
    }
    
        
    void mpi_comm_thread:: start(mpi::Requests *req) throw()
    {
        assert(req!=NULL);
        _start(req);
    }
    
    void mpi_comm_thread:: _start(mpi::Requests *req) throw()
    {
        
        //----------------------------------------------------------------------
        //-- get a lock on a ready thread
        //----------------------------------------------------------------------
        for(;;)
        {
            if( access.try_lock() )
            {
                if( ready )
                {
                    break;
                }
                access.unlock();
            }
        }
        MPI.Printf(stderr, "Got a lock: start requests@%p\n", req);

        //----------------------------------------------------------------------
        //-- I got the lock: set process parameter
        //----------------------------------------------------------------------
        requests = req;
        
        //-- prepare the process to run
        enter.signal();
        
        //-- release the mutex -> start the process
        ready = false;
        access.unlock();
    }
    
    void mpi_comm_thread:: loop() throw()
    {
        { scoped_lock guard(access); MPI.Printf(stderr, "In Thread\n"); }
        assert(false == ready );
        
        //----------------------------------------------------------------------
        // lock the access
        //----------------------------------------------------------------------
    CYCLE:
        access.lock();
        ready = true;
        MPI.Printf(stderr,"** Thread is ready\n");
        
        //----------------------------------------------------------------------
        // wait ON A LOCKED MUTEX
        //----------------------------------------------------------------------
        enter.wait( access );
        
        //----------------------------------------------------------------------
        // Here, I am LOCKED
        //----------------------------------------------------------------------
        assert(false==ready);
        MPI.Printf(stderr, "** Will run requests@%p\n", requests);
        
        //----------------------------------------------------------------------
        // Ready to unlock
        //----------------------------------------------------------------------
        access.unlock();
        
        if(requests)
        {
            MPI.Waitall( *requests );
            goto CYCLE;
        }
        // return
        
    }
    
}