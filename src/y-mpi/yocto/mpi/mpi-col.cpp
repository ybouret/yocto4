#include "yocto/mpi/mpi.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/ptr/auto.hpp"

#include <cstring>

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
    
    namespace
    {
        typedef memory::buffer_of<char,memory::pooled> buffer_type;
    }
    
    void mpi::Bcast( string &s, int root, MPI_Comm comm) const
    {
        //----------------------------------------------------------------------
        // Broadcasting size: only root counts !!
        //----------------------------------------------------------------------
        size_t sz = s.size();
        Bcast<size_t>(sz,root,comm);
        
        //----------------------------------------------------------------------
        // Prepare data
        //----------------------------------------------------------------------
        auto_ptr<buffer_type> buff;
        char                 *addr = 0;
        
        memory::buffer_of<char,memory::pooled> buf( sz );
        
        
        const bool is_root = (root == Comm_rank(comm));
        if(is_root)
        {
            addr = &s[0];
        }
        else
        {
            buff.reset( new buffer_type(sz) );
            addr = (*buff)(0);
        }
        
        //----------------------------------------------------------------------
        // Broadcast effectively
        //----------------------------------------------------------------------
        Bcast(addr, sz, MPI_BYTE, root, comm);
        
        //----------------------------------------------------------------------
        // Rebuild strings
        //----------------------------------------------------------------------
        if(!is_root)
        {
            const string tmp( addr, sz);
            s = tmp;
        }
    }
    
    
	
}

