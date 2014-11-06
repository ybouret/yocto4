#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/mpi/mpi.hpp"

#include "yocto/ios/ocstream.hpp"


using namespace yocto;
using namespace fame;


int main(int argc, char *argv[])
{
    
    bool pbc[3] = { true, true, true };
    
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
            
            const layout1D full(1,100);
            RectilinearMesh<layout1D,double> mesh(adb,"m2",full,pbc,rank,size,2);
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
            
            const int rv = rank * 1024;
            C.ld( rv  );
            D.ld( rv );
            
            // fill ghosts
            {
                int shift = 1;
                for( quad_ghosts<layout1D> *g = mesh.local_ghosts.head;g;g=g->next,++shift)
                {
                    C.fill( rv + shift, g->source.zone );
                    C.fill( rv - shift, g->target.zone );
                   
                    D.fill( rv + shift, g->source.zone );
                    D.fill( rv - shift, g->target.zone );

                }
                
                for( quad_ghosts<layout1D> *g = mesh.async_ghosts.head;g;g=g->next,++shift)
                {
                    C.fill( rv + shift, g->source.zone );
                    C.fill( rv - shift, g->target.zone );
                    
                    D.fill( rv + shift, g->source.zone );
                    D.fill( rv - shift, g->target.zone );
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
            
            // data exchange
            MPI.Printf0(stderr, "Exchanging...\n");
            for( quad_ghosts<layout1D> *g = mesh.local_ghosts.head;g;g=g->next)
            {
                g->local_update(handles);
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