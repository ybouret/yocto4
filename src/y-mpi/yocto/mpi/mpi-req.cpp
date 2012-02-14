#include "yocto/mpi/mpi.hpp"
#include "yocto/memory/global.hpp"

namespace yocto
{
	
	mpi::Requests:: ~Requests() throw()
	{
		memory::kind<memory::global>::release( wksp_, wlen_ );
		request = 0;
		status  = 0;
	}
	
	
	mpi:: Requests:: Requests( size_t num ) :
	count( num ),
	wlen_( count * ( sizeof(MPI_Request) + sizeof(MPI_Status) ) ),
	wksp_( memory::kind<memory::global>::acquire( wlen_ ) ),
	request( 0 ),
	status(  0 )
	{
		// memory for requests and status is allocated
		uint8_t *p = (uint8_t *)wksp_;
		
		request    = (MPI_Request *) (void*) &p[0];
		p += count * sizeof(MPI_Request);
	
		status     = (MPI_Status  *) (void*) &p[0];
		
	}

	
	void mpi:: Startall( Requests &requests ) const
	{
		if( requests.count )
			Startall( requests.count, &requests[0] );
	}
	
	void mpi:: Waitall( Requests &requests ) const
	{
		if( requests.count )
			Waitall( requests.count, &requests[0], &requests(0) );
	}
	
	MPI_Request & mpi:: Requests:: operator[]( size_t indx ) throw()
	{
		assert( indx < count );
		return request[indx];
	}
	
	MPI_Status & mpi:: Requests:: operator()( size_t indx) const throw()
	{
		assert( indx < count );
		return status[indx];
	}
	
	
}
