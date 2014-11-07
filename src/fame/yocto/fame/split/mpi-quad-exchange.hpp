#ifndef YOCTO_FAME_SPLIT_MPI_QUAD_EXCHANGE_INCLUDED
#define YOCTO_FAME_SPLIT_MPI_QUAD_EXCHANGE_INCLUDED 1

#include "yocto/fame/mesh/quad-grid.hpp"
#include "yocto/mpi/mpi.hpp"

namespace yocto
{
    namespace fame
    {
        
        struct mpi_quad_exchange
        {
            
            template <typename QUADGRID>  static inline
            void data(QUADGRID       &grid,
                      linear_handles &handles,
                      const mpi      &MPI)
            {
                MPI.Printf0(stderr, "\tlocal exchange\n");
                //! first pass: local exchange
                for(const typename QUADGRID::Ghosts *g = grid.local_ghosts.head;g;g=g->next)
                {
                    g->local_update(handles);
                }
                
                
                //! exchange by dimension
                for(size_t dim=0;dim<QUADGRID::DIMENSIONS;++dim)
                {
                    MPI.Printf0(stderr, "\tasync exchange %s\n", get_axis_name(dim));
                    MPI_Request request[4];
                    size_t ir = 0;
                    typename QUADGRID::GhostsPtrs &asyncs = grid.asynchronous[dim];
                    
                    // prepare sending one way
                    for(int i=0;i<asyncs.count;++i)
                    {
                        typename QUADGRID::Ghosts &g = asyncs[i];
                        g.send_assemble(handles);
                        MPI.Isend(g.sbuffer, g.stored, MPI_BYTE, g.peer, 0, MPI_COMM_WORLD, request[ir++]);
                    }
                    
                    // prepare receiving the other way
                    for(int i=asyncs.count-1;i>=0;--i)
                    {
                        typename QUADGRID::Ghosts &g = asyncs[i];
                        MPI.Irecv(g.rbuffer, g.stored, MPI_BYTE, g.peer, 0, MPI_COMM_WORLD, request[ir++]);
                    }
                    
                    MPI_Status status[4];
                    MPI.Waitall(ir, request, status);
                    
                    // decompact
                    for(int i=0;i<asyncs.count;++i)
                    {
                        typename QUADGRID::Ghosts &g = asyncs[i];
                        g.recv_dispatch(handles);
                    }
                    
                }
            }
            
        };
        
    }
}

#endif
