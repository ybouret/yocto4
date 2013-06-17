#ifndef YOCTO_MPI_INCLUDED
#define YOCTO_MPI_INCLUDED 1

#include "yocto/exception.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/code/printf-check.hpp"
#include "yocto/code/endian.hpp"

#include "yocto/string.hpp"
#include "yocto/type-spec.hpp"
#include "yocto/sequence/vector.hpp"

#include <cstdio>

#if !defined(_MSC_VER)
#define YOCTO_CPLUSPLUS __cplusplus
#undef __cplusplus
#endif
extern "C" {
#include <mpi.h>
}
#if !defined(_MSC_VER)
#define __cplusplus YOCTO_CPLUSPLUS
#endif

namespace yocto
{
	/*
	 point to point
	 @MPI_Bsend				@MPI_Bsend_init			@MPI_Buffer_attach
	 @MPI_Buffer_detach 	MPI_Cancel				@MPI_Get_count
	 @MPI_Get_elements  	MPI_Ibsend				MPI_Iprobe
	 MPI_Irecv				MPI_Irsend				@MPI_Isend
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
	 MPI_Allgather			MPI_Allgatherv  	@MPI_Allreduce
	 MPI_Alltoall			MPI_Alltoallv		@MPI_Barrier
	 @MPI_Bcast				@MPI_Gather			MPI_Gatherv
	 MPI_Op_create			MPI_Op_free			@MPI_Reduce
	 MPI_Reduce_scatter 	MPI_Scan			@MPI_Scatter
	 MPI_Scatterv
	 */
	
	//! MPI functions wrappers
	class mpi : public singleton<mpi>
	{
	public:
		
		//! dedicated error handling
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
		
        
		const int        CommWorldSize;    //!< size of MPI_COMM_WORLD
		const int        CommWorldRank;    //!< rank in MPI_COMM_WORLD
		const int        CommWorldLast;    //!< CommWorldSize-1;
		const bool       IsFirst;          //!< 0 == CommWorldRank
        const bool       IsFinal;          //!< CommWorldLast == CommWorldRank
        const bool       IsParallel;       //!< CommWorldSize > 1
        mutable uint64_t CommTime;         //!< cumulative communication time in microseconds
		const int        ProcessorNameLength;
		const char       ProcessorName[MPI_MAX_PROCESSOR_NAME]; //!< from MPI_Get_Processor_name(...)
		const int        ThreadLevel;
        const string     CommWorldID;
        
        const char *ThreadLevelName() const throw();
        int Comm_rank( MPI_Comm comm ) const;
        
        
		//======================================================================
		// Point-to-point Communication Routines
		//======================================================================
		void       Send(  const void *buffer, size_t count, MPI_Datatype datatype, int dest,  int tag, MPI_Comm comm ) const;
		void       Rsend( const void *buffer, size_t count, MPI_Datatype datatype, int dest,  int tag, MPI_Comm comm ) const;
		void       Bsend( const void *buffer, size_t count, MPI_Datatype datatype, int dest,  int tag, MPI_Comm comm ) const;
		void       Ssend( const void *buffer, size_t count, MPI_Datatype datatype, int dest,  int tag, MPI_Comm comm ) const;
		void       Recv(  void       *buffer, size_t count, MPI_Datatype datatype, int source,int tag, MPI_Comm comm, MPI_Status &status) const;
		void       Isend( const void *buffer, size_t count, MPI_Datatype datatype, int dest,  int tag, MPI_Comm comm, MPI_Request &request) const;
		void       Irecv( void       *buffer, size_t count, MPI_Datatype datatype, int source,int tag, MPI_Comm comm, MPI_Request &request) const;
		
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
		
        void      Sendrecv(const void *sendbuf, size_t sendcount, MPI_Datatype sendtype, int dest, int sendtag,
                           void       *recvbuf, size_t recvcount, MPI_Datatype recvtype, int from, int recvtag,
                           MPI_Comm    comm,
                           MPI_Status &status ) const;
        //======================================================================
        // Collective Communication Routines
        //======================================================================
        void Barrier(MPI_Comm comm) const;
        void Bcast( void *buffer, size_t count, MPI_Datatype datatype, int root, MPI_Comm comm ) const;
        void Scatter(   const void *sendbuf, size_t sendcnt, MPI_Datatype sendtype, void *recvbuf, size_t recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm ) const;
        void Gather(    const void *sendbuf, size_t sendcnt, MPI_Datatype sendtype, void *recvbuf, size_t recvcnt, MPI_Datatype recvtype, int root, MPI_Comm comm ) const;
        void Reduce(    const void *sendbuf, void *recvbuf, size_t count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) const;
        void Allreduce( const void *sendbuf, void *recvbuf, size_t count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) const;
        
        //======================================================================
        // Usefull helpers
        //======================================================================
        int  CommWorldNext() const throw(); //!< modulus the CommWorldSize
        int  CommWorldPrev() const throw(); //!< modulus the CommWorldSize
		
        //======================================================================
        // stdio helpers
        //======================================================================
        
        //! parallel printf, in order in MPI_COMM_WORLD
        void Printf( FILE *fp, const char *fmt, ... ) const YOCTO_PRINTF_CHECK(3,4);
        
        //! printf only on rank 0
        void Printf0( FILE *fp, const char *fmt, ... ) const YOCTO_PRINTF_CHECK(3,4);
        
        //! close standard I/O on rank>0
        void CloseStdIO() const;
        
        //! MPI_Request/MPI_Status helper
        class Requests
		{
		public:
			explicit Requests( size_t num );
			virtual ~Requests() throw();
			const size_t count;
			MPI_Request &operator[]( size_t index ) throw();       //!< 0 <= index <= count-1
			MPI_Status  &operator()( size_t index ) const throw(); //!< 0 <= index <= count-1
			
		private:
			size_t       wlen_;
			void        *wksp_;
			MPI_Request *request;
			MPI_Status  *status;
			YOCTO_DISABLE_COPY_AND_ASSIGN(Requests);
		};
		
		void Startall( Requests & ) const;
		void Waitall( Requests & ) const;
		
        
        //======================================================================
        // Send/Recv templated for integral types
        //======================================================================
        
        //! send ONE integral type
        template <typename T>
        inline void Send( const T x, int dest,  int tag, MPI_Comm comm ) const
        {
            const T y = swap_be_as<T>(x);
            this->Send(&y, sizeof(T), MPI_BYTE, dest, tag, comm);
        }
        
        //! recv ONE integral type
        template <typename T>
        inline T Recv( int source,int tag, MPI_Comm comm, MPI_Status &status) const
        {
            T y(0);
            this->Recv(&y, sizeof(T), MPI_BYTE, source, tag, comm, status);
            return swap_be_as<T>(y);
        }
        
        
        
        //! bcast ONE integral type
        template <typename T>
        inline void Bcast( T &x, int root, MPI_Comm comm ) const
        {
            T y = swap_be_as<T>(x);
            Bcast(&y, sizeof(T), MPI_BYTE, root, comm);
            x = swap_be_as<T>(y);
        }
        
        //! send ONE integral type
        template <typename T>
        inline void Isend( const T x, int dest,  int tag, MPI_Comm comm, MPI_Request &req) const
        {
            const T y = swap_be_as<T>(x);
            this->Isend(&y, sizeof(T), MPI_BYTE, dest, tag, comm, req);
        }
        
        double Wtime() const throw();
        void   WaitFor( double nsec) const throw();
        
        //======================================================================
        //
        // Send/Recv for strings
        //
        //======================================================================
        void   Send( const string &s, int dest, int tag, MPI_Comm comm ) const;
        string Recv( int source, int tag, MPI_Comm comm, MPI_Status &status ) const;
        
        //======================================================================
        //
        // data type
        //
        //======================================================================
        class db_item
        {
        public:
            const type_spec    spec;
            const MPI_Datatype id;
            
            db_item( const std::type_info &info, const MPI_Datatype ID) throw();
            ~db_item() throw();
            db_item(const db_item &other) throw();
            
            static int compare( const db_item &lhs, const db_item &rhs ) throw();
            
            
        private:
            YOCTO_DISABLE_ASSIGN(db_item);
        };
        
        MPI_Datatype get_type( const std::type_info &info ) const;
        MPI_Datatype get_type( const type_spec      &spec ) const;
        template <typename T>
        inline MPI_Datatype get_type() const
        {
            return get_type( typeid(T) );
        }
        
        const vector<db_item> db;
        
	private:
		friend class singleton<mpi>;                           //!< access mpi
		static const threading::longevity life_time = 0;       //!< TODO: set to a better value
		static const char                 name[];
		explicit mpi();
		virtual ~mpi() throw();
		
		
		YOCTO_DISABLE_COPY_AND_ASSIGN(mpi);
		void clear_pname() throw();
		void on_finalize() throw(); //!< clean up
        
        
	};
	
	
}

#define YOCTO_MPI   const mpi & MPI = mpi::init(&argc,&argv)

#define MPI_CTIME_DEBUG 0

#if defined(MPI_CTIME_DEBUG) && MPI_CTIME_DEBUG == 1
#include <iostream>
#define Y_MPI_STAMP(FUNC) const double stamp = MPI_Wtime(); std::cerr << "[+" << #FUNC << "]" << std::endl
#else
#define Y_MPI_STAMP(FUNC) const double stamp = MPI_Wtime()
#endif
#define Y_MPI_CTIME CommTime += uint64_t( ( MPI_Wtime() - stamp ) * 1.0e6 )
#endif

#define Y_MPI_TIME64(TMX,MPI,CODE) \
do { const uint64_t mpi_enter = MPI.CommTime; { CODE; } TMX += (MPI.CommTime - mpi_enter); } while(false)

