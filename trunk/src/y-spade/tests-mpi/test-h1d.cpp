#include "yocto/utest/run.hpp"
#include "yocto/spade/mpi/exchange.hpp"
#include "yocto/spade/workspace.hpp"
#include "yocto/spade/rmesh.hpp"

#include "../tests/main.hpp"

YOCTO_UNIT_TEST_IMPL(h1d)
{
    const mpi &    MPI  = mpi::init( &argc, &argv );
    const int      rank = MPI.CommWorldRank;
    const int      size = MPI.CommWorldSize;
    const layout1D full_layout(0,100);
    const layout1D sim_layout( full_layout.split(rank,size) );
    MPI.Printf( stderr, "%d.%d: %d->%d, #=%d\n", size, rank, int(sim_layout.lower), int(sim_layout.upper), int(sim_layout.width) );
    
    fields_setup<layout1D> F(2);
    Y_SPADE_FIELD(F,"A" , array1D<double> );
    Y_SPADE_LOCAL(F,"LA", array1D<double> );
    ghosts_setup G;
    if( rank < size ) G.set_async( ghost::at_upper_x, 1, MPI.CommWorldNext() );
    if( rank > 0    ) G.set_async( ghost::at_lower_x, 1, MPI.CommWorldPrev() );
    
    
    
    
}
YOCTO_UNIT_TEST_DONE()
