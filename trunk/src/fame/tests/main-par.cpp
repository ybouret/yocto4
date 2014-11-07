#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/mpi/mpi.hpp"

#include "yocto/ios/ocstream.hpp"


using namespace yocto;
using namespace fame;


int main(int argc, char *argv[])
{
    
    bool pbc[3] = { true, true, true };
    for(int i=1;i<argc;++i)
    {
        if( strcmp(argv[i],"-x") == 0 )
        {
            pbc[0] = false;
            continue;
        }
        
        if( strcmp(argv[i],"-y") == 0 )
        {
            pbc[1] = false;
            continue;
        }
        
        if( strcmp(argv[i],"-z") == 0 )
        {
            pbc[2] = false;
            continue;
        }
        
        
    }
    
    try
    {
        YOCTO_MPI_ENV();
        arrays adb;
        const int    rank = MPI.CommWorldRank;
        const int    size = MPI.CommWorldSize;
        const string pfx  = vformat("%d.%d", size, rank);
        
        //______________________________________________________________________
        //
        // in 1 D
        //______________________________________________________________________
        {
            
            const layout1D full(1,16);
            RectilinearMesh<layout1D,double> mesh(adb,"m2",full,pbc,rank,size,1);
            for(unit_t i=mesh.outline.lower;i<=mesh.outline.upper;++i)
            {
                mesh.X()[i] = i;
            }
            MPI.Printf(stderr, "layout:  %d->%d\n", (int)mesh.lower, (int)mesh.upper);
            MPI.Newline(stderr);
            MPI.Printf(stderr, "outline: %d->%d\n", (int)mesh.outline.lower, (int)mesh.outline.upper);
            MPI.Newline(stderr);
            MPI.Printf(stderr, "#local ghost: %2d | #async ghost: %2d\n", (int) mesh.local_ghosts.size, (int) mesh.async_ghosts.size );
            MPI.Printf(stderr, "#local async x ghosts: %d\n", int(mesh.asynchronous[0].count));
            adb.store( new array1D<double>("C", mesh.outline) );
            adb.store( new array1D<float>("D",mesh.outline)   );
            
            array1D<double> &C = adb["C"].as< array1D<double> >();
            array1D<float>  &D = adb["D"].as< array1D<float>  >();
            
            
            for(unit_t i=mesh.lower;i<=mesh.upper;++i)
            {
                C[i] = 1+i;
                D[i] = 1+i;
            }
            
            
            // fill ghosts
            {
                for( quad_ghosts<layout1D> *g = mesh.local_ghosts.head;g;g=g->next)
                {
                    //C.fill( -rank, g->source.zone );
                    C.fill( -rank, g->target.zone );
                    
                    //D.fill(  -rank, g->source.zone );
                    D.fill(  -rank, g->target.zone );
                    
                }
                
                for( quad_ghosts<layout1D> *g = mesh.async_ghosts.head;g;g=g->next)
                {
                    //C.fill( -rank, g->source.zone );
                    C.fill( -rank, g->target.zone );
                    
                    //D.fill(  -rank, g->source.zone );
                    D.fill(  -rank, g->target.zone );
                }
            }
            
            
            {
                ios::ocstream fp( "ini" + pfx + ".dat", false);
                for(unit_t i=mesh.outline.lower;i<=mesh.outline.upper;++i)
                {
                    fp("%g %g %g\n", mesh.X()[i], C[i], D[i]);
                }
            }
            
            linear_handles handles;
            handles.append( &C );
            handles.append( &D );
            
            mesh.allocate_async_for(handles.chunk_size);
            
            // data exchange
            MPI.Printf0(stderr, "Exchanging...\n");
            
            // local
            for( quad_ghosts<layout1D> *g = mesh.local_ghosts.head;g;g=g->next)
            {
                g->local_update(handles);
            }
            
            // one dim
            {
                quad_ghosts<layout1D>::pointers &asyncs = mesh.asynchronous[0];
                mpi::Requests requests(asyncs.count*2);
                size_t ir = 0;
                //sending in one direction
                for(int i=0;i<asyncs.count;++i)
                {
                    quad_ghosts<layout1D> &g = asyncs[i];
                    g.send_assemble(handles);
                    
                    const size_t bytes = g.items * handles.chunk_size;
                    MPI.Isend(g.sbuffer, bytes, MPI_BYTE, g.peer, 0, MPI_COMM_WORLD, requests[ir++]);
                    //MPI.Irecv(g.rbuffer, bytes, MPI_BYTE, g.peer, 0, MPI_COMM_WORLD, requests[ir++]);
                }
                
                // receiving from the other direction
                for(int i=asyncs.count-1;i>=0;--i)
                {
                    quad_ghosts<layout1D> &g = asyncs[i];
                    MPI.Irecv(g.rbuffer, g.items * handles.chunk_size, MPI_BYTE, g.peer, 0, MPI_COMM_WORLD, requests[ir++]);
                }
                
                MPI.Waitall(requests);
                
                for(size_t i=0;i<asyncs.count;++i)
                {
                    quad_ghosts<layout1D> &g = asyncs[i];
                    g.recv_dispatch(handles);
                }
            }
            
            {
                ios::ocstream fp( "end" + pfx + ".dat", false);
                for(unit_t i=mesh.outline.lower;i<=mesh.outline.upper;++i)
                {
                    fp("%g %g %g\n", mesh.X()[i], C[i], D[i]);
                }
            }
            
        }
        
        
        return 0;
    }
    catch(...)
    {
        std::cerr << "Uncaught Exception!" << std::endl;
    }
    return 1;
}