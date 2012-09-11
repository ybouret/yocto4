#include "yocto/utest/run.hpp"
#include "yocto/spade/mpi/workspace.hpp"
#include "yocto/spade/rmesh.hpp"

#include "../tests/main.hpp"

#define LX (1.2)
#define NX (100)

YOCTO_UNIT_TEST_IMPL(h1d)
{
    const mpi &    MPI  = mpi::init( &argc, &argv );
    const int      rank = MPI.CommWorldRank;
    const int      size = MPI.CommWorldSize;
    const int      last = size-1;
    const layout1D full_layout(0,NX);
    const layout1D sim_layout( full_layout.split(rank,size) );
    MPI.Printf( stderr, "%d.%d: %d->%d, #=%d\n", size, rank, int(sim_layout.lower), int(sim_layout.upper), int(sim_layout.width) );
    
    fields_setup<layout1D> F(2);
    Y_SPADE_FIELD(F,"A" , array1D<double> );
    Y_SPADE_LOCAL(F,"LA", array1D<double> );
    ghosts_setup G;
    if( rank < last ) G.set_async( ghost::at_upper_x, 1, MPI.CommWorldNext() );
    if( rank > 0    ) G.set_async( ghost::at_lower_x, 1, MPI.CommWorldPrev() );
    
    mpi_workspace<layout1D, rmesh,double> W(sim_layout,F,G);
    
    array1D<double> &X  = W["X"].as< array1D<double> >();
    array1D<double> &A  = W["A"].as< array1D<double> >();
    array1D<double> &LA = W["LA"].as< array1D<double> >();

    const double     deltaX = LX/NX;
    const double     dX2    = deltaX * deltaX;
    const double     dt     = 0.1 * dX2;
    
    for( unit_t i = X.lower; i <= X.upper;++i ) X[i] = (i*LX)/NX;
    MPI.Printf( stderr, "%d.%d: lower=%d, upper=%d\n", size, rank, int(A.lower), int(A.upper) );
    
    //-- initialize boundary condition
    if( 0 == rank )
    {
        assert( A.lower == 0 );
        A[A.lower] = 1.0;
    }
    if( last == rank )
    {
        assert( A.upper == NX );
        A[A.upper] = 0;
    }
    
    //-- sync A
    W.sync(MPI);
    
    //-- loop
    for(;;)
    {
       
        //-- compute laplacian
        for( unit_t i=A.lower+1;i<A.upper;++i)
        {
            LA[i] = (A[i-1] - 2*A[i] + A[i+1])/dX2;
        }
        
        //-- update field
        for(unit_t i=A.lower+1;i<A.upper;++i)
        {
            A[i] += dt * LA[i];
        }
        
        //-- sync A
        W.sync(MPI);
        
        break;
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
