#ifndef YOCTO_MPI_COMM_THREAD_INCLUDED
#define YOCTO_MPI_COMM_THREAD_INCLUDED 1

#include "yocto/mpi/mpi.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/threading/thread.hpp"
#include "yocto/auto-clean.hpp"

namespace yocto
{
    
    class mpi_comm_thread
    {
    public:
        explicit mpi_comm_thread( const mpi &ref );
        virtual ~mpi_comm_thread() throw();
        
        
        //!
        /**
         \warning requests must be created BEFORE mpi_comm_thread
         */
        void     start( mpi::Requests *req ) throw(); //!< !NULL
        void     stop() throw();
        
    private:
        const mpi                &MPI;
        threading::mutex          access; //!< MPI.access
        bool                      ready;  //!< internal ready flag
        threading::condition      enter;  //!< enter condition
        mpi::Requests            *requests;
        threading::thread         thr;
        
        static void       call(void*) throw();
        void              loop() throw();
        
        void _start( mpi::Requests *req) throw(); //!< NULL => stop
    };
    
    
}

#endif
