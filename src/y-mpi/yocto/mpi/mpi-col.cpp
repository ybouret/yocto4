#include "yocto/mpi/mpi.hpp"

namespace yocto 
{
	
	
	//======================================================================
	// Collective Communication Routines
	//======================================================================
	void mpi:: Barrier(MPI_Comm comm) const
	{
        Y_MPI_STAMP(Barrier);
		const int err = MPI_Barrier( comm );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Barrier()" );
		Y_MPI_CTIME;
	}
	
	void mpi:: Bcast( void *buffer, size_t count, MPI_Datatype datatype, int root, MPI_Comm comm ) const
	{
        Y_MPI_STAMP(Bcast);
		const int err = MPI_Bcast( (void *)buffer, count, datatype, root, comm );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Bcast(root=%d)", root );
        Y_MPI_CTIME;
	}
	
	
	void mpi:: Scatter( const void *sendbuf, size_t sendcnt, MPI_Datatype sendtype, void *recvbuf, size_t recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm ) const
	{
		assert( !(NULL==sendbuf && sendcnt > 0 ) );
		assert( !(NULL==recvbuf && recvcnt > 0 ) );
        Y_MPI_STAMP(Scatter);
		const int err = MPI_Scatter((void*)sendbuf,sendcnt,sendtype,recvbuf,recvcnt,recvtype,root,comm);
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Scatter(root=%d)", root );
        Y_MPI_CTIME;
	}

	
	void mpi:: Gather( const void *sendbuf, size_t sendcnt, MPI_Datatype sendtype, void *recvbuf, size_t recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm ) const
	{
		assert( !(NULL==sendbuf && sendcnt > 0 ) );
		assert( !(NULL==recvbuf && recvcnt > 0 ) );
        Y_MPI_STAMP(Gather);
		const int err = MPI_Gather((void*)sendbuf,sendcnt,sendtype,recvbuf,recvcnt,recvtype,root,comm);
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Gather(root=%d)", root );
        Y_MPI_CTIME;
	}
	
    void mpi:: Reduce(  const void *sendbuf, void *recvbuf, size_t count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) const
    {
        assert( !(NULL==sendbuf && count>0 ) );
        assert( !(NULL==recvbuf && count>0 ) );
        Y_MPI_STAMP(Reduce);
        const int err = MPI_Reduce((void*)sendbuf, recvbuf, int(count), datatype, op, root, comm);
        if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Reduce(root=%d)",root);
        Y_MPI_CTIME;
    }
    
    void mpi:: Allreduce(const void *sendbuf, void *recvbuf, size_t count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) const
    {
        assert( !(NULL==sendbuf && count>0 ) );
        assert( !(NULL==recvbuf && count>0 ) );
        Y_MPI_STAMP(Allreduce);
        const int err = MPI_Allreduce((void*)sendbuf, recvbuf, int(count), datatype, op, comm);
        if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Alleduce()");
        Y_MPI_CTIME;
    }

    
	
}

