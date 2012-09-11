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
            
            inline void init_exchange(const mpi &MPI)
            {
                mpi_exchange::init(MPI, self, requests);
            }
            
            inline void wait_exchange(const mpi &MPI)
            {
                mpi_exchange::wait(MPI,self,requests);
            }
            
            inline void sync(const mpi &MPI)
            {
                init_exchange(MPI);
                wait_exchange(MPI);
            }
            
        private:
            dataspace<LAYOUT> &self;
            mpi::Requests      requests;
            YOCTO_DISABLE_COPY_AND_ASSIGN(mpi_workspace);
        };
        
        
    }
    
}

#endif
