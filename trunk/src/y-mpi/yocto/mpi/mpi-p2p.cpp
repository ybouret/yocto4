#include "yocto/mpi/mpi.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/memory/pooled.hpp"

namespace yocto
{
	
	
#define YOCTO_MPI_XSEND(SEND) \
void mpi:: SEND( const void *buffer, size_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm ) const \
{                                                   \
assert( !( NULL == buffer && count > 0 ) );         \
Y_MPI_STAMP(SEND);                                  \
const int err = MPI_##SEND( (void *)buffer, int(count), datatype, dest, tag, comm );\
if( err != MPI_SUCCESS )                            \
throw mpi::exception( err, #SEND"(dest=%d)", dest );\
Y_MPI_CTIME;                                        \
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
        Y_MPI_STAMP(Recv);
		const int err = MPI_Recv( (void *)buffer, int(count), datatype, source, tag, comm, &status );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Recv(source=%d)",source );
        Y_MPI_CTIME;
	}
	
	size_t mpi:: Get_count( const MPI_Status *status, MPI_Datatype datatype ) const
	{
		assert( NULL != status );
        Y_MPI_STAMP(Get_count);
		int       count = 0;
		const int err   = MPI_Get_count( (MPI_Status *)status, datatype, &count );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Get_count()");
        Y_MPI_CTIME;
		assert( count >= 0 );
		return count;
	}
	
	size_t mpi:: Get_elements( const MPI_Status *status, MPI_Datatype datatype ) const
	{
		assert( NULL != status );
        Y_MPI_STAMP(Get_elements);
		int       count = 0;
		const int err   = MPI_Get_elements( (MPI_Status *)status, datatype, &count );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Get_elements()");
		assert( count >= 0 );
        Y_MPI_CTIME;
		return count;
	}
	
#define YOCTO_MPI_XSEND_INIT(SEND) \
void mpi:: SEND( const void *buffer, size_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request &request) const \
{\
assert( !( NULL == buffer && count > 0 ) );\
Y_MPI_STAMP(SEND);\
const int err = MPI_##SEND( (void *)buffer, int(count), datatype, dest, tag, comm, &request );\
if( err != MPI_SUCCESS )\
throw mpi::exception( err, "MPI_" #SEND "()" );\
Y_MPI_CTIME;\
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
        Y_MPI_STAMP(Recv_init);
		const int err = MPI_Recv_init( buffer, count, datatype, source, tag, comm, &request );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Recv_init(source=%d)",source );
        Y_MPI_CTIME;
	}
	
	void mpi:: Start( MPI_Request &request ) const
	{
		const int err = MPI_Start( &request );
        Y_MPI_STAMP(Start);
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Start()" );
        Y_MPI_CTIME;
	}
	
	void   mpi:: Startall( size_t count, MPI_Request requests[] ) const
	{
        Y_MPI_STAMP(Startall);
		const int err = MPI_Startall( count, requests );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Startall()" );
        Y_MPI_CTIME;
	}
	
	void mpi:: Wait( MPI_Request &request, MPI_Status &status ) const
	{
        Y_MPI_STAMP(Wait);
		const int err = MPI_Wait( &request, &status );
		if( err != MPI_SUCCESS )
		{
			throw mpi::exception( err, "MPI_Wait()" );
		}
        Y_MPI_CTIME;
	}
	
	void mpi:: Waitall( size_t count, MPI_Request requests[], MPI_Status status[] ) const
	{
		assert( !( count >0 && requests == NULL ) );
		assert( !( count >0 && status   == NULL ) );
        Y_MPI_STAMP(Waitall);
		const int err = MPI_Waitall( count, requests, status );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "MPI_Waitall()" );
        Y_MPI_CTIME;
	}
	
	
	void mpi:: Isend( const void *buffer, size_t count, MPI_Datatype datatype, int dest,  int tag, MPI_Comm comm, MPI_Request &request) const
	{
		assert( !( NULL == buffer && count > 0 ) );
        Y_MPI_STAMP(Isend);
		const int err = MPI_Isend( (void *)buffer, int(count), datatype, dest, tag, comm, &request );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "Isend()" );
        Y_MPI_CTIME;
	}
	
	void  mpi:: Irecv( void       *buffer, size_t count, MPI_Datatype datatype, int source,int tag, MPI_Comm comm, MPI_Request &request) const
	{
		assert( !( NULL == buffer && count > 0 ) );
        Y_MPI_STAMP(Irecv);
		const int err = MPI_Irecv( buffer, int(count), datatype, source, tag, comm, &request );
		if( err != MPI_SUCCESS )
			throw mpi::exception( err, "Irecv()" );
        Y_MPI_CTIME;
		
	}
	
	void  mpi:: Sendrecv(const void *sendbuf, size_t sendcount, MPI_Datatype sendtype, int dest, int sendtag,
                         void       *recvbuf, size_t recvcount, MPI_Datatype recvtype, int from, int recvtag,
                         MPI_Comm    comm,
                         MPI_Status &status ) const
    {
        assert( !(sendbuf==0&&sendcount>0));
        assert( !(recvbuf==0&&recvcount>0));
        
        Y_MPI_STAMP(Sendrecv);
        const int err = MPI_Sendrecv((void*)sendbuf, int(sendcount), sendtype, dest, sendtag,
                                     recvbuf,        int(recvcount), recvtype, from, recvtag,
                                     comm, &status);
        if( err != MPI_SUCCESS )
			throw mpi::exception( err, "Sendrecv(dest=%d,from=%d)", dest, from );
        Y_MPI_CTIME;
    }
    
    
    //==========================================================================
    // send/recv strings helper
    //==========================================================================
	void mpi::Send(const string &s, int dest, int tag, MPI_Comm comm) const
    {
        //-- send the size
        const size_t len = s.size();
        Send<size_t>(len,dest,tag,comm);
        
        //-- send the content
        if(len>0)
        {
            Send( s.ro(), len, MPI_BYTE, dest, tag, comm );
        }
    }
    
    string mpi::Recv(int source, int tag, MPI_Comm comm, MPI_Status &status) const
    {
        //-- recv the size
        const size_t len = Recv<size_t>(source, tag, comm, status);
        
        //-- recv the content
        if( len > 0 )
        {
            memory::buffer_of<char,memory::pooled> buf( len );
            Recv(buf.rw(), len, MPI_BYTE, source, tag, comm, status);
            return string(buf(),len);
        }
        else
            return string();
    }
	
       
}
