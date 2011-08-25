#include "yocto/mpi/mpi.hpp"

namespace yocto 
{
	
	
#define YOCTO_MPI_XSEND(SEND) \
void mpi:: SEND( const void *buffer, size_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm ) const \
{\
assert( !( NULL == buffer && count > 0 ) );\
const int err = MPI_##SEND( (void *)buffer, int(count), datatype, dest, tag, comm );\
if( err != MPI_SUCCESS )\
throw mpi::exception( err, #SEND"()" );\
}
	
	
	//======================================================================
	// Collective Communication Routines
	//======================================================================
	YOCTO_MPI_XSEND(Send)	
	YOCTO_MPI_XSEND(Rsend)
	YOCTO_MPI_XSEND(Bsend)
	YOCTO_MPI_XSEND(Ssend)
	
	void       mpi::Recv( void       *buffer, size_t count, MPI_Datatype datatype, int source,int tag, MPI_Comm comm, MPI_Status &status) const
	{
		assert( !(NULL==buffer && count > 0 ) );
		const int err = MPI_Recv( (void *)buffer, int(count), datatype, source, tag, comm, &status );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Send()" );
	}
	
	size_t mpi:: Get_count( const MPI_Status *status, MPI_Datatype datatype ) const
	{
		assert( NULL != status );
		int       count = 0;
		const int err   = MPI_Get_count( (MPI_Status *)status, datatype, &count );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Get_count()");
		assert( count >= 0 );
		return count;
	}
	
	size_t mpi:: Get_elements( const MPI_Status *status, MPI_Datatype datatype ) const
	{
		assert( NULL != status );
		int       count = 0;
		const int err   = MPI_Get_elements( (MPI_Status *)status, datatype, &count );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Get_elements()");
		assert( count >= 0 );
		return count;
	}
	
#define YOCTO_MPI_XSEND_INIT(SEND) \
void mpi:: SEND( const void *buffer, size_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request &request) const \
{\
assert( !( NULL == buffer && count > 0 ) );\
const int err = MPI_##SEND( (void *)buffer, int(count), datatype, dest, tag, comm, &request );\
if( err != MPI_SUCCESS )\
throw mpi::exception( err, "MPI_" #SEND "()" );\
}
	
	YOCTO_MPI_XSEND_INIT(Send_init)
	YOCTO_MPI_XSEND_INIT(Rsend_init)
	YOCTO_MPI_XSEND_INIT(Bsend_init)
	YOCTO_MPI_XSEND_INIT(Ssend_init)
	
	void mpi:: Buffer_attach( void *buffer, size_t length ) const
	{
		assert( !( NULL == buffer && length > 0 ) );
		const int err = MPI_Buffer_attach(buffer, int(length) );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Buffer_attach(%p,%d)", buffer, int(length) );
	}
	
	size_t mpi:: Buffer_detach( void *buffer ) const
	{
		int size = 0;
		const int err = MPI_Buffer_detach( buffer, & size );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Buffer_detach(%p)", buffer );
		return size;
	}
	
	void      mpi:: Recv_init( void* buffer, size_t count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request &request) const
	{
		assert( !( NULL == buffer && count > 0 ) );
		const int err = MPI_Recv_init( buffer, count, datatype, source, tag, comm, &request );
		if( err != MPI_SUCCESS )
		{
			throw mpi::exception( err, "MPI_Recv_init" );
		}
	}
	
	void mpi:: Start( MPI_Request &request ) const
	{
		const int err = MPI_Start( &request );
		if( err != MPI_SUCCESS )
		{
			throw mpi::exception( err, "MPI_Start()" );
		}
	}
	
	void   mpi:: Startall( size_t count, MPI_Request requests[] ) const
	{
		const int err = MPI_Startall( count, requests );
		if( err != MPI_SUCCESS )
		{
			throw mpi::exception( err, "MPI_Startall()" );
		}
	}
	
	void mpi:: Wait( MPI_Request &request, MPI_Status &status ) const
	{
		const int err = MPI_Wait( &request, &status );
		if( err != MPI_SUCCESS )
		{
			throw mpi::exception( err, "MPI_Wait()" );
		}
	}
	
	void mpi:: Waitall( size_t count, MPI_Request requests[], MPI_Status status[] ) const
	{
		assert( !( count >0 && requests == NULL ) );
		assert( !( count >0 && status   == NULL ) );
		const int err = MPI_Waitall( count, requests, status );
		if( err != MPI_SUCCESS )
		{
			throw mpi::exception( err, "MPI_Waitall()" );
		}
	}
	
	
}
