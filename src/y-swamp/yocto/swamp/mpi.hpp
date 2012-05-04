#ifndef YOCTO_SWAMP_MPI_INCLUDED
#define YOCTO_SWAMP_MPI_INCLUDED 1


#include "yocto/swamp/dataspace.hpp"
#include "yocto/mpi/mpi.hpp"

namespace yocto 
{
    
    namespace swamp
    {
        
        
        struct _mpi
        {
            template <typename LAYOUT> inline static 
            void init_exchange( mpi &MPI, dataspace<LAYOUT> &D, mpi::Requests &requests, int tag = 100)
            {
                assert( requests.count == D.num_requests() );
                const linear_handles &handles = D.handles();
                
                //==============================================================
                // exchange local ghosts
                //==============================================================
                for( size_t i= D.local_ghosts_count(); i>0; --i )
                {
                    D.__local_ghosts(i).transfer( handles );
                }
                
                //==============================================================
                // start exchange of async ghost
                //==============================================================
                size_t iRequest = 0;
                for( size_t i = D.async_ghosts_count(); i>0; --i)
                {
                    async_ghosts &g = D.__async_ghosts(i);
                    assert(g.peer>=0);
                    assert(g.peer<MPI.CommWorldSize);
                    assert(g.length>0);
                    assert(g.inner_buf!=NULL);
                    assert(g.outer_buf!=NULL);
                    
                    //----------------------------------------------------------
                    //  Non blocking recv from peer
                    //----------------------------------------------------------
                    MPI.Irecv(g.outer_buf, g.length, MPI_BYTE , g.peer, tag, MPI_COMM_WORLD, requests[iRequest++]);
                    assert(iRequest<requests.count);
                    //----------------------------------------------------------
                    //  Fill inner ghost with data
                    //----------------------------------------------------------
                    g.store_inner( handles );
                    
                    //----------------------------------------------------------
                    //  Non blocking send to peer
                    //----------------------------------------------------------
                    MPI.Isend(g.inner_buf, g.length, MPI_BYTE, g.peer, tag, MPI_COMM_WORLD, requests[iRequest++]);
                    assert( iRequest<=requests.count );
                }
                assert( requests.count == iRequest );
            }
            
            template <typename LAYOUT> inline static 
            void wait_exchange( mpi &MPI, dataspace<LAYOUT> &D, mpi::Requests &requests)
            {
                assert( requests.count == D.num_requests() );
                const linear_handles &handles = D.handles();
                MPI.Waitall(requests);
                for( size_t i = D.async_ghosts_count(); i>0; --i)
                {
                    async_ghosts &g = D.__async_ghosts(i);
                    assert(g.peer>=0);
                    assert(g.peer<MPI.CommWorldSize);
                    assert(g.length>0);
                    assert(g.inner_buf!=NULL);
                    assert(g.outer_buf!=NULL);
                    
                    //----------------------------------------------------------
                    //  Fill dataspace with outer ghost data
                    //----------------------------------------------------------
                    g.query_outer(handles);
                }
            }
            
        };
        
    }
}
#endif
