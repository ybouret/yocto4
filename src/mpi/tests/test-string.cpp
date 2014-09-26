#include "yocto/utest/run.hpp"

#include "yocto/mpi/mpi.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(string)
{
	const int tag = 1010;
	
    YOCTO_MPI_ENV();
    MPI.CloseStdIO();
	
    if( MPI.IsParallel )
    {
        if( MPI.IsFirst )
        {
            fprintf( stderr, "From Node %d.%d\n", MPI.CommWorldSize, MPI.CommWorldRank);
            for( int i=1; i < MPI.CommWorldSize; ++i )
            {
                MPI_Status status;
                const string msg = MPI.Recv(i, tag, MPI_COMM_WORLD, status);
                fprintf( stderr, "msg> '%s'\n", msg.c_str());
            }
        }
        else
        {
            const string msg = vformat( "I'm the node %d.%d!", MPI.CommWorldSize, MPI.CommWorldRank);
            MPI.Send(msg, 0, tag, MPI_COMM_WORLD);
        }
    }
    
    MPI.Barrier(MPI_COMM_WORLD);
    MPI.WaitFor(0.2);
	
	
}

YOCTO_UNIT_TEST_DONE()
