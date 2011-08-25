#ifndef YOCTO_MPI_INCLUDED
#define YOCTO_MPI_INCLUDED 1

#include "yocto/exception.hpp"

#include "yocto/threading/singleton.hpp"

#undef __cplusplus
extern "C" {
#include <mpi.h>
}
#define __cplusplus


namespace yocto
{
	/*
	 point to point
	 @MPI_Bsend				@MPI_Bsend_init			@MPI_Buffer_attach
	 @MPI_Buffer_detach 	MPI_Cancel				@MPI_Get_count
	 @MPI_Get_elements  	MPI_Ibsend				MPI_Iprobe
	 MPI_Irecv				MPI_Irsend				MPI_Isend
	 MPI_Issend				MPI_Probe				@MPI_Recv
	 @MPI_Recv_init			MPI_Request_free		@MPI_Rsend
	 @MPI_Rsend_init		@MPI_Send				@MPI_Send_init
	 MPI_Sendrecv			MPI_Sendrecv_replace 	@MPI_Ssend
	 @MPI_Ssend_init		@MPI_Start				@MPI_Startall
	 MPI_Test				MPI_Test_cancelled		MPI_Testall
	 MPI_Testany			MPI_Testsome			@MPI_Wait
	 @MPI_Waitall			MPI_Waitany				MPI_Waitsome
	 */
	
	
	/* collective:
	 MPI_Allgather			MPI_Allgatherv  	MPI_Allreduce
	 MPI_Alltoall			MPI_Alltoallv		@MPI_Barrier
	 @MPI_Bcast				MPI_Gather			MPI_Gatherv
	 MPI_Op_create			MPI_Op_free			MPI_Reduce
	 MPI_Reduce_scatter 	MPI_Scan			MPI_Scatter
	 MPI_Scatterv 	  	  
	 */
	
	class mpi : public singleton<mpi>
	{
	public:
		
		//! error handling
		class exception : public yocto::exception
		{
		public:
			explicit exception( int err, const char *fmt,... ) throw();
			exception( const exception & ) throw();
			virtual ~exception() throw();
			
			virtual const char *what() const throw(); //< Uses MPI_Error_string(...) to format the error code.
			
			const int code;
			
		private:
			YOCTO_DISABLE_ASSIGN(exception);
			char string_[MPI_MAX_ERROR_STRING];
		};
		
		static mpi & init( int * argc, char ***argv );
		void   Finalize() throw();
		
		
		const int CommWorldSize;
		const int CommWorldRank;
		const int CommWorldRankMax; //!< CommWorldSize-1;
		
		const int    ProcessorNameLength;
		const char   ProcessorName[MPI_MAX_PROCESSOR_NAME];         //!< from MPI_Get_Processor_name(...)
		
		//======================================================================
		// Point-to-point Communication Routines
		//======================================================================
		void       Send(  const void *buffer, size_t count, MPI_Datatype datatype, int dest,  int tag, MPI_Comm comm ) const;
		void       Rsend( const void *buffer, size_t count, MPI_Datatype datatype, int dest,  int tag, MPI_Comm comm ) const;
		void       Bsend( const void *buffer, size_t count, MPI_Datatype datatype, int dest,  int tag, MPI_Comm comm ) const;
		void       Ssend( const void *buffer, size_t count, MPI_Datatype datatype, int dest,  int tag, MPI_Comm comm ) const;
		void       Recv(  void       *buffer, size_t count, MPI_Datatype datatype, int source,int tag, MPI_Comm comm, MPI_Status &status) const;
		
		size_t     Get_count( const MPI_Status *status, MPI_Datatype datatype ) const;
		size_t     Get_elements( const MPI_Status *status, MPI_Datatype datatype ) const;
		
		void      Send_init(  const void* buf, size_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request &request) const;
		void      Rsend_init( const void* buf, size_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request &request) const;
		void      Bsend_init( const void* buf, size_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request &request) const;
		void      Ssend_init( const void* buf, size_t count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request &request) const;
		
		void      Recv_init( void* buf, size_t count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request &request) const;
		
		void      Buffer_attach( void *buffer, size_t length ) const;
		size_t    Buffer_detach( void *buffer ) const;
		
		void      Start( MPI_Request &request ) const;
		void      Startall( size_t count, MPI_Request requests[] ) const;
		
		void      Wait( MPI_Request &request, MPI_Status &status ) const;
		void      Waitall( size_t count, MPI_Request requests[], MPI_Status status[] ) const;
		
		//======================================================================
		// Collective Communication Routines
		//======================================================================
		void Barrier(MPI_Comm comm) const;
		void Bcast( const void *buffer, size_t count, MPI_Datatype datatype, int root, MPI_Comm comm ) const;
		
		//======================================================================
		// Usefull helpers
		//======================================================================
		int CommWorldNext() const throw(); //!< CommWorldSize should be >= 2
		int CommWorldPrev() const throw(); //!< CommWorldSize should be >= 2
		
	private:
		friend class singleton<mpi>;                            //!< access mpi
		static const threading::longevity life_time = 0;       //!< TODO: set to a better value
		static const char                 name[];
		explicit mpi();
		virtual ~mpi() throw();
		
		
		YOCTO_DISABLE_COPY_AND_ASSIGN(mpi);
		void clear_pname() throw();
		void on_finalize() throw(); //!< clean up
	};
	
	
}

#endif
