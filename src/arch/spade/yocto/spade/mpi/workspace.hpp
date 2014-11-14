#if 1
#ifndef YOCTO_SPADE_MPI_WORKSPACE_INCLUDED
#define YOCTO_SPADE_MPI_WORKSPACE_INCLUDED 1

#include "yocto/spade/workspace.hpp"
#include "yocto/spade/mpi/exchange.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        template <
        typename LAYOUT,
        template <class,class> class MESH,
        typename U>
        class mpi_workspace : public workspace<LAYOUT,MESH,U>
        {
        public:
            explicit mpi_workspace(const LAYOUT               &L,
                                   const fields_setup<LAYOUT> &F,
                                   const ghosts_setup         &G) :
            workspace<LAYOUT,MESH,U>(L,F,G),
            self( *this ),
            requests( this->num_requests )
            {
                
            }
            
            virtual ~mpi_workspace() throw() {}
            
            inline void init_exchange(const mpi &MPI, linear_handles &handles)
            {
                mpi_exchange::init(MPI,handles,self,requests);
            }
            
            inline void wait_exchange(const mpi &MPI, linear_handles &handles)
            {
                mpi_exchange::wait(MPI,handles,self,requests);
            }
            
            inline void sync(const mpi &MPI, linear_handles &handles)
            {
                mpi_exchange::sync(MPI,handles,self,requests);
            }
            
            inline void sync_bw(const mpi &MPI,
                                linear_handles &handles,
                                unsigned long &io_bytes,
                                double        &io_time)
            {
                mpi_exchange::sync_bw(MPI,handles,self,requests, io_bytes, io_time);
            }
            
            inline void sync1(const mpi &MPI, linear &handle)
            {
                mpi_exchange::sync1(MPI,handle,self,requests);
            }
            
        private:
            dataspace<LAYOUT> &self;
            mpi::Requests      requests;
            YOCTO_DISABLE_COPY_AND_ASSIGN(mpi_workspace);
        };
        
        
    }
    
}

#endif
#endif
