#include "yocto/fame/mesh/rectilinear.hpp"
#include "yocto/mpi/mpi.hpp"


using namespace yocto;
using namespace fame;


int main(int argc, char *argv[])
{
    
    bool pbc[3] = { true, true, true };
    
    try
    {
        YOCTO_MPI_ENV();
        arrays adb;
        const int rank = MPI.CommWorldRank;
        const int size = MPI.CommWorldSize;
        
        //______________________________________________________________________
        //
        // in 1 D
        //______________________________________________________________________
        {
            
            const layout1D full(1,100);
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
            
            adb.store( new array1D<double>("C", mesh.outline) );
        
            array1D<double> &C = adb["C"].as< array1D<double> >();
            
            C.ld( rank * 1024 );
            for( quad_ghosts<layout1D> *g = mesh.local_ghosts.head; g; g=g->next)
            {
                
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