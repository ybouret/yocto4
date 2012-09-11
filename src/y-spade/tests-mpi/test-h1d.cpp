#include "yocto/utest/run.hpp"
#include "yocto/spade/mpi.hpp"
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
    MPI.Printf( stderr, "%d.%d: %d->%d\n", size, rank, int(sim_layout.lower), int(sim_layout.upper) );
}
YOCTO_UNIT_TEST_DONE()
