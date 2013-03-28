#include "yocto/mpi/comm-thread.hpp"

#include <iostream>

namespace yocto
{
    
    mpi_comm_thread:: ~mpi_comm_thread() throw()
    {
        thr.join();
    }
    
    mpi_comm_thread:: mpi_comm_thread( const mpi &ref ) :
    MPI(ref),
    access(MPI.access),
    ready(false),
    onair(false),
    thr( mpi_comm_thread::launch, this)
    {
        //scoped_lock guard(access);
        
    }
    
    
    void  mpi_comm_thread:: launch(void *args) throw()
    {
        assert(args);
        static_cast<mpi_comm_thread *>(args)->process();
    }
    
    bool mpi_comm_thread:: is_ready() const throw()
    {
        while( ! access.try_lock() )
            ;
        const bool ans = ready;
        access.unlock();
        return ans;
    }
    
    void mpi_comm_thread:: start(int flag)
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
        std::cerr << "got a lock: start...flag=" << flag << std::endl;

        //----------------------------------------------------------------------
        //-- I got the lock: set process parameter
        //----------------------------------------------------------------------
        todo = flag;
        
        //-- prepare the process to run
        enter.signal();
        
        //-- release the mutex -> start the process
        ready = false;
        access.unlock();
    }
    
    void mpi_comm_thread:: process() throw()
    {
        { scoped_lock guard(access); std::cerr << "In Thread@rank=" << MPI.CommWorldRank << std::endl; }
        assert(false == ready );
        assert(false == onair );
        
        //----------------------------------------------------------------------
        // lock the access
        //----------------------------------------------------------------------
    CYCLE:
        access.lock();
        ready = true;
        std::cerr << "** Thread@rank=" << MPI.CommWorldRank << " is ready" << std::endl;
        
        //----------------------------------------------------------------------
        // wait ON A LOCKED MUTEX
        //----------------------------------------------------------------------
        enter.wait( access );
        
        //----------------------------------------------------------------------
        // Here, I am LOCKED
        //----------------------------------------------------------------------
        assert(false==ready);
        std::cerr << "** Will run rank=" << MPI.CommWorldRank << " with todo=" << todo << std::endl;
        
        //----------------------------------------------------------------------
        // 
        //----------------------------------------------------------------------
        access.unlock();
        
        if(todo>0)
            goto CYCLE;
        
        // return
        
    }
    
}