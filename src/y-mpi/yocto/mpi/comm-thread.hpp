#ifndef YOCTO_MPI_COMM_THREAD_INCLUDED
#define YOCTO_MPI_COMM_THREAD_INCLUDED 1

#include "yocto/mpi/mpi.hpp"
#include "yocto/threading/condition.hpp"
#include "yocto/threading/thread.hpp"

namespace yocto
{
    
    class mpi_comm_thread
    {
    public:
        explicit mpi_comm_thread( const mpi &ref );
        virtual ~mpi_comm_thread() throw();
        
        bool     is_ready() const throw();
        void     start(int flag);
        
        
    private:
        const mpi                &MPI;
        threading::mutex         &access; //!< MPI.access
        bool                      ready;  //!< internal ready flag
        bool                      onair;  //!< internal I/O flag
        threading::condition      enter;  //!< enter condition 
        threading::thread         thr;
        int                       todo;
        
        static void       launch(void*) throw();
        void              process() throw();
    };
    
    
}

#endif
