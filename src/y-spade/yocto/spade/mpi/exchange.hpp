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
            ////////////////////////////////////////////////////////////////////
            //
            // Multiple/Interleaved Exchanges
            //
            ////////////////////////////////////////////////////////////////////
            
            template <typename DATASPACE>
            static inline
            void init(const mpi      &MPI,
                      linear_handles &handles,
                      DATASPACE      &ds,
                      mpi::Requests  &requests )
            {
                static const int tag = 0xCA44;
                assert( requests.count == ds.num_requests );
                
                //--------------------------------------------------------------
                // transfert local ghosts : PBC are ready after that
                //--------------------------------------------------------------
                for( size_t i=ds.local_count;i>0;--i)
                    ds.get_local(i).transfer( handles );
                
                //--------------------------------------------------------------
                // load data and create recv requests
                //--------------------------------------------------------------
                size_t       iRequest = 0;
                const size_t na       = ds.async_count;
                for( size_t i=na;i>0;--i)
                {
                    async_ghosts &g = ds.get_async(i);
                    assert(g.peer>=0);
                    assert(g.peer<MPI.CommWorldSize);
                    assert(g.ibuffer!= 0);
                    assert(g.obuffer!= 0);
                    assert(g.iobytes > 0);
                    
                    //----------------------------------------------------------
                    // create ibuffer and update g.content to exchanged #bytes
                    //----------------------------------------------------------
                    g.inner_store( handles );
                    
                    //----------------------------------------------------------
                    // create non blocking recv
                    //----------------------------------------------------------
                    MPI.Irecv(g.obuffer,
                              g.content,
                              MPI_BYTE,
                              g.peer,
                              tag,
                              MPI_COMM_WORLD,
                              requests[iRequest++]);
                    assert(iRequest<=requests.count);
                    
                }
                
                //--------------------------------------------------------------
                // create send requests in reverse order
                //--------------------------------------------------------------
                for( size_t i=1; i <= na; ++i )
                {
                    async_ghosts &g = ds.get_async(i);
                    //----------------------------------------------------------
                    // create non blocking send
                    //----------------------------------------------------------
                    MPI.Isend(g.ibuffer,
                              g.content,
                              MPI_BYTE,
                              g.peer,
                              tag,
                              MPI_COMM_WORLD,
                              requests[iRequest++]);
                    assert(iRequest<=requests.count);
                }
                assert(iRequest==requests.count);
            }
            
            template <typename DATASPACE> static inline
            void wait(const mpi      & MPI,
                      linear_handles & handles,
                      DATASPACE      & ds,
                      mpi::Requests  & requests )
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
                    g.outer_query(handles);
                }
            }
            
            template <typename DATASPACE>
            static inline
            void sync(const mpi      & MPI,
                      linear_handles & handles,
                      DATASPACE      & ds,
                      mpi::Requests  & requests )
            {
                init(MPI,handles,ds,requests);
                wait(MPI,handles,ds,requests);
            }
            
            ////////////////////////////////////////////////////////////////////
            //
            // Single Field Exchange
            //
            ////////////////////////////////////////////////////////////////////
            template <typename DATASPACE>
            static inline
            void init1( const mpi &MPI, linear &handle, DATASPACE &ds, mpi::Requests &requests )
            {
                static const int tag = 0xCA441;
                assert( requests.count == ds.num_requests );
                
                //--------------------------------------------------------------
                // transfert local ghosts
                //--------------------------------------------------------------
                for( size_t i=ds.local_count;i>0;--i)
                {
                    ds.get_local(i).transfer( handle );
                }
                
                //--------------------------------------------------------------
                // load send data and create recv requests
                //--------------------------------------------------------------
                const size_t na = ds.async_count;
                if(na>0)
                {
                    size_t       iRequest = 0;
                    for( size_t i=na;i>0;--i)
                    {
                        async_ghosts &g = ds.get_async(i);
                        assert(g.peer>=0);
                        assert(g.peer<MPI.CommWorldSize);
                        assert(g.ibuffer != 0);
                        assert(g.obuffer != 0);
                        assert(g.iobytes >  0);
                        
                        //------------------------------------------------------
                        // create ibuffer
                        //------------------------------------------------------
                        g.inner_store( handle );
                        
                        //------------------------------------------------------
                        // create non blocking recv
                        //------------------------------------------------------
                        MPI.Irecv(g.obuffer,
                                  g.content,
                                  MPI_BYTE,
                                  g.peer,
                                  tag,
                                  MPI_COMM_WORLD,
                                  requests[iRequest++]);
                        assert(iRequest<=requests.count);
                        
                    }
                    
                    //--------------------------------------------------------------
                    // create send requests in reverse order
                    //--------------------------------------------------------------
                    for( size_t i=1;i<=na;++i)
                    {
                        async_ghosts &g = ds.get_async(i);
                        //----------------------------------------------------------
                        // create non blocking send
                        //----------------------------------------------------------
                        MPI.Isend(g.ibuffer,
                                  g.content,
                                  MPI_BYTE,
                                  g.peer,
                                  tag,
                                  MPI_COMM_WORLD,
                                  requests[iRequest++]);
                        assert(iRequest<=requests.count);
                    }
                    
                    
                }
            }
            
            template <typename DATASPACE> static inline
            void wait1(const mpi     & MPI,
                       linear        & handle,
                       DATASPACE     & ds,
                       mpi::Requests & requests )
            {
                assert( requests.count == ds.num_requests );
                const size_t na = ds.async_count;
                if(na>0)
                {
                    //----------------------------------------------------------
                    // wait for MPI I/O
                    //----------------------------------------------------------
                    MPI.Waitall(requests);
                    
                    //----------------------------------------------------------
                    // dispatch ghosts data
                    //----------------------------------------------------------
                    for( size_t i=na;i>0;--i)
                    {
                        async_ghosts &g = ds.get_async(i);
                        g.outer_query(handle);
                    }
                }
            }
            
            template <typename DATASPACE> static inline
            void sync1( const mpi & MPI, linear &handle, DATASPACE &ds, mpi::Requests &requests )
            {
                init1(MPI,handle,ds,requests);
                wait1(MPI,handle,ds,requests);
            }
            
            ////////////////////////////////////////////////////////////////////
            //
            // Sync With Bandwith
            //
            ////////////////////////////////////////////////////////////////////
            
            
            template <typename DATASPACE>
            static inline
            void sync_bw(const mpi      & MPI,
                         linear_handles & handles,
                         DATASPACE      & ds,
                         mpi::Requests  & requests,
                         unsigned long  & io_bytes,
                         double         & io_time
                         )
            {
                static const int tag = 0xCA44;
                //==============================================================
                //
                // Initialize Requests
                //
                //==============================================================
                assert( requests.count == ds.num_requests );
                
                //----------------------------------------------------------
                // transfert local ghosts : PBC are ready after that
                //----------------------------------------------------------
                for( size_t i=ds.local_count;i>0;--i)
                    ds.get_local(i).transfer( handles );
                
                //----------------------------------------------------------
                // load data
                //----------------------------------------------------------
                const size_t na       = ds.async_count;
                for( size_t i=na;i>0;--i)
                {
                    async_ghosts &g = ds.get_async(i);
                    assert(g.peer>=0);
                    assert(g.peer<MPI.CommWorldSize);
                    assert(g.ibuffer!= 0);
                    assert(g.obuffer!= 0);
                    assert(g.iobytes > 0);
                    //------------------------------------------------------
                    // create ibuffer and update
                    // g.content to exchanged #bytes
                    //------------------------------------------------------
                    g.inner_store( handles );
                    io_bytes += g.content;
                }
                
                //----------------------------------------------------------
                // prepare recv requests
                //----------------------------------------------------------
                size_t       iRequest = 0;
                const double io_enter = MPI.Wtime();
                for( size_t i=na;i>0;--i)
                {
                    async_ghosts &g = ds.get_async(i);
                    
                    //------------------------------------------------------
                    // create non blocking recv
                    //------------------------------------------------------
                    MPI.Irecv(g.obuffer,
                              g.content,
                              MPI_BYTE,
                              g.peer,
                              tag,
                              MPI_COMM_WORLD,
                              requests[iRequest++]);
                    assert(iRequest<=requests.count);
                }
                
                //--------------------------------------------------------------
                // create send requests in reverse order
                //--------------------------------------------------------------
                for( size_t i=1; i <= na; ++i )
                {
                    async_ghosts &g = ds.get_async(i);
                    //----------------------------------------------------------
                    // create non blocking send
                    //----------------------------------------------------------
                    MPI.Isend(g.ibuffer,
                              g.content,
                              MPI_BYTE,
                              g.peer,
                              tag,
                              MPI_COMM_WORLD,
                              requests[iRequest++]);
                    assert(iRequest<=requests.count);
                }
                assert(iRequest==requests.count);
                
                //==============================================================
                //
                // Wait/Dispatch
                //
                //==============================================================
                assert( requests.count == ds.num_requests );
                //--------------------------------------------------------------
                // wait for MPI I/O
                //--------------------------------------------------------------
                MPI.Waitall(requests);
                const double io_leave = MPI.Wtime();
                //--------------------------------------------------------------
                // dispatch ghosts data
                //--------------------------------------------------------------
                for( size_t i=ds.async_count;i>0;--i)
                {
                    async_ghosts &g = ds.get_async(i);
                    g.outer_query(handles);
                }
                
                io_time += (io_leave-io_enter);
            }
            
            
        };
    }
}

#endif
