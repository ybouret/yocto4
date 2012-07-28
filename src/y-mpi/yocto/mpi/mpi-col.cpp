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
	
	void mpi:: Bcast( void *buffer, size_t count, MPI_Datatype datatype, int root, MPI_Comm comm ) const
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
	
    void mpi:: Reduce(  const void *sendbuf, void *recvbuf, size_t count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) const
    {
        assert( !(NULL==sendbuf && count>0 ) );
        assert( !(NULL==recvbuf && count>0 ) );
        const int err = MPI_Reduce((void*)sendbuf, recvbuf, int(count), datatype, op, root, comm);
        if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Reduce(root=%d)",root);
    }
    
    void mpi:: Allreduce(     void *sendbuf, void *recvbuf, size_t count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) const
    {
        assert( !(NULL==sendbuf && count>0 ) );
        assert( !(NULL==recvbuf && count>0 ) );
        const int err = MPI_Allreduce((void*)sendbuf, recvbuf, int(count), datatype, op, comm);
        if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Alleduce()");

    }

    
	
}

