#include "yocto/mpi/mpi.hpp"

namespace yocto 
{
	
	
	//======================================================================
	// Collective Communication Routines
	//======================================================================
	void mpi:: Barrier(MPI_Comm comm) const
	{
		const int err = MPI_Barrier( comm );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Barrier()" );
		
	}
	
	void mpi:: Bcast( const void *buffer, size_t count, MPI_Datatype datatype, int root, MPI_Comm comm ) const
	{
		
		const int err = MPI_Bcast( (void *)buffer, count, datatype, root, comm );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Bcast()" );
	}
	
	
}

