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
			throw mpi::exception( err, "MPI_Bcast(root=%d)", root );
	}
	
	
	void mpi:: Scatter( const void *sendbuf, size_t sendcnt, MPI_Datatype sendtype, void *recvbuf, size_t recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm ) const
	{
		//assert( !(NULL==sendbuf && sendcnt > 0 ) );
		assert( !(NULL==recvbuf && recvcnt > 0 ) );
		const int err = MPI_Scatter((void*)sendbuf,sendcnt,sendtype,recvbuf,recvcnt,recvtype,root,comm);
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Scatter(root=%d)", root );
	}

	
	void mpi:: Gather( const void *sendbuf, size_t sendcnt, MPI_Datatype sendtype, void *recvbuf, size_t recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm ) const
	{
		assert( !(NULL==sendbuf && sendcnt > 0 ) );
		assert( !(NULL==recvbuf && recvcnt > 0 ) );
		const int err = MPI_Gather((void*)sendbuf,sendcnt,sendtype,recvbuf,recvcnt,recvtype,root,comm);
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Gather(root=%d)", root );
	}
	
	
}

