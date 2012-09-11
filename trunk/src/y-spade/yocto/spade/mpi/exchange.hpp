#ifndef YOCTO_SPADE_MPI_EXCHANGE_INCLUDED
#define YOCTO_SPADE_MPI_EXCHANGE_INCLUDED 1

#include "yocto/spade/dataspace.hpp"
#include "yocto/mpi/mpi.hpp"

namespace yocto
{
    namespace spade
    {
        struct mpi_exchange
        {
            
            
            template <typename DATASPACE> static inline
            void init( const mpi & MPI, DATASPACE &ds, mpi::Requests requests )
            {
                static const int tag = 0xCA44;
                assert( requests.count == ds.num_requests );
                
                //--------------------------------------------------------------
                // transfert local ghosts
                //--------------------------------------------------------------
                for( size_t i=ds.local_count;i>0;--i)
                    ds.get_local(i).transfer( ds.handles );
                
                //--------------------------------------------------------------
                // load and create send requests
                //--------------------------------------------------------------
                size_t iRequest = 0;
                for( size_t i=ds.async_count;i>0;--i)
                {
                    async_ghosts &g = ds.get_async(i);
                    assert(g.peer>=0);
                    assert(g.peer<MPI.CommWorldSize);
                    assert(g.ibuffer!=0);
                    assert(g.obuffer!=0);
                    assert(g.iobytes>0);
                    
                    //----------------------------------------------------------
                    // create ibuffer
                    //----------------------------------------------------------
                    const size_t ns = g.inner_store( ds.handles );
                    
                    //----------------------------------------------------------
                    // create non blocking recv
                    //----------------------------------------------------------
                    MPI.Irecv(g.obuffer, ns, MPI_BYTE, g.peer, tag, MPI_COMM_WORLD, requests[iRequest++]);
                    assert(iRequest<=requests.count);
                    
                    //----------------------------------------------------------
                    // create non blocking send
                    //----------------------------------------------------------
                    MPI.Isend(g.ibuffer, ns, MPI_BYTE, g.peer, tag, MPI_COMM_WORLD, requests[iRequest++]);
                    assert(iRequest<=requests.count);
                }
                
                //--------------------------------------------------------------
                // process with MPI
                //--------------------------------------------------------------
                MPI.Startall(requests);
            }
            
            template <typename DATASPACE> static inline
            void wait( const mpi & MPI, DATASPACE &ds, mpi::Requests requests )
            {
                assert( requests.count == ds.num_requests );
                //--------------------------------------------------------------
                // wait for MPI I/O
                //--------------------------------------------------------------
                MPI.Waitall(requests);
                
                //--------------------------------------------------------------
                // dispatch ghosts data
                //--------------------------------------------------------------
                for( size_t i=ds.async_count;i>0;--i)
                {
                    async_ghosts &g = ds.get_async(i);
                    g.outer_query(ds.handles);
                }
            }
            
            template <typename DATASPACE> static inline
            void sync( const mpi & MPI, DATASPACE &ds, mpi::Requests requests )
            {
                init(MPI,ds,requests);
                wait(MPI,ds,requests);
            }
            
            
            
        };
    }
}

#endif
