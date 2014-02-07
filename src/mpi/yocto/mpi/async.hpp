#ifndef YOCTO_MPI_ASYNC_INCLUDED
#define YOCTO_MPI_ASYNC_INCLUDED 1

#include "yocto/mpi/mpi.hpp"
#include "yocto/threading/barrier.hpp"
#include "yocto/threading/threads.hpp"

namespace yocto
{
    
    class mpi_async
    {
    public:
        explicit mpi_async( const mpi &);
        virtual ~mpi_async() throw();
        
        //! requests must be available throughout
        void launch( mpi::Requests &req );
        void finish() throw();
        
    private:
        const mpi            & MPI;
        bool                   ready;
        mpi::Requests         *requests;
        threading::threads     workers; //!< only 1...
        threading::mutex      &access;
        threading::condition   enter;
        threading::barrier     leave;
        YOCTO_DISABLE_COPY_AND_ASSIGN(mpi_async);

        void clear() throw();
        
        static void call(void *) throw();
        void        loop(void)   throw();
        void        stop() throw();

        void        lock_on_ready() throw();
        void        load_requests(mpi::Requests*) throw();
        void        resume_thread() throw();
    };
    
}

#endif

