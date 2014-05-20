#include "yocto/mpi/mpi.hpp"

#include <cstring>
#include <cstdarg>

namespace yocto
{
	
       
    
    
    //==========================================================================
    //
    // mpi exceptions
    //
    //==========================================================================
#if defined(_MSC_VER)
	// init of string_
#pragma warning ( disable : 4351 )
#endif
	mpi:: exception:: exception( int err, const char *fmt, ... ) throw() :
	yocto::exception(),
	code( err ),
	string_()
	{
		va_list ap;
		va_start(ap,fmt);
		format(fmt,&ap);
		memset( string_, 0, sizeof(string_) );
		
	}
	
	mpi:: exception:: exception( const mpi::exception &e ) throw() :
	yocto::exception(e),
	code( e.code ),
	string_()
	{
		memcpy( string_, e.string_, sizeof(string_) );
	}
	
	mpi:: exception:: ~exception() throw()
	{
	}
	
	const char * mpi:: exception:: what() const throw()
	{
		return string_;
	}
	
	//==========================================================================
	namespace
	{
		static int *    mpi_argc_ = NULL;
		static char *** mpi_argv_ = NULL;
		static void mpi_reset_() throw()
		{
			mpi_argc_ = NULL;
			mpi_argv_ = NULL;
		}
	}
	
	void mpi:: clear_pname() throw()
	{
		*(int *) &ProcessorNameLength = 0;
		memset( (void *) ProcessorName, 0, sizeof(ProcessorName) );
	}
	
	void mpi:: on_finalize() throw()
	{
		*(int *) & CommWorldSize    = 0;
		*(int *) & CommWorldRank    = 0;
		*(int *) & CommWorldLast    = 0;
		clear_pname();
		(void) MPI_Finalize();
	}
	
    const char *mpi:: ThreadLevelName() const throw()
    {
        switch( ThreadLevel )
        {
            case MPI_THREAD_SINGLE:        return "MPI_THREAD_SINGLE";
            case MPI_THREAD_FUNNELED:      return "MPI_THREAD_FUNNELED";
            case MPI_THREAD_SERIALIZED:    return "MPI_THREAD_SERIALIZED";
            case MPI_THREAD_MULTIPLE:      return "MPI_THREAD_MULTIPLE";
            default:
                break;
        }
        return "UNKNOWN";
        
    }
    
    int mpi:: RequestedThreadLevel = MPI_THREAD_SINGLE;
    
	mpi:: mpi() :
	CommWorldSize(0),
	CommWorldRank(0),
	CommWorldLast(0),
    IsFirst(true),
    IsFinal(true),
    IsParallel(false),
    CommTime(0),
	ProcessorNameLength(0),
	ProcessorName(),
    ThreadLevel(0),
    CommWorldID(),
    db(32,as_capacity),
	hasher()
	{
		if( NULL == mpi_argc_ || NULL == mpi_argv_ )
		{
			mpi_reset_();
			throw yocto::exception("no possible direct mpi::instance()");
		}
		
		
		try
		{
            
            //==================================================================
            // MPI basic setup
            //==================================================================
			int err = MPI_Init_thread( mpi_argc_, mpi_argv_ , RequestedThreadLevel, (int*)&ThreadLevel);
			if( err != MPI_SUCCESS )
			{
				throw mpi::exception( err, "MPI_Init_thread()");
			}
			
			err = MPI_Comm_size( MPI_COMM_WORLD, (int *) & CommWorldSize );
			if( err != MPI_SUCCESS )
			{
				throw mpi::exception( err, "MPI_Comm_size(MPI_COMM_WORLD)");
			}
			
			(int  &) CommWorldLast = CommWorldSize - 1;
			(bool &) IsParallel    = CommWorldSize > 1;
            
			err = MPI_Comm_rank( MPI_COMM_WORLD, (int *) & CommWorldRank );
			if( err != MPI_SUCCESS )
			{
				throw mpi::exception( err, "MPI_Comm_rank(MPI_COMM_WORLD)");
			}
			assert( CommWorldRank <= CommWorldLast );
			(bool &)IsFirst = (CommWorldRank == 0);
			(bool &)IsFinal = (CommWorldRank == CommWorldLast);
            
            //==================================================================
            // MPI advanced setup
            //==================================================================
			clear_pname();
			err = MPI_Get_processor_name( (char *) ProcessorName, (int *) & ProcessorNameLength );
			if( err != MPI_SUCCESS )
			{
				clear_pname();
                // TODO: throw an exception ?
			}
            
            gendb();
            
            (string &)CommWorldID = vformat("%d.%d", CommWorldSize, CommWorldRank);
        }
        catch(...)
        {
            mpi_reset_();
            on_finalize();
            throw;
        }
    }
    
    mpi & mpi:: init( int * argc, char ***argv )
    {
        YOCTO_LOCK(access);
        if( NULL == argc || NULL == argv )
        {
            throw yocto::exception("yocto::mpi::Init( INVALID ARGS )");
        }
        
        mpi_argc_ = argc;
        mpi_argv_ = argv;
        mpi & ans = mpi::instance();
        mpi_reset_();
        return ans;
    }
    
    
    void mpi:: Finalize() throw()
    {
        destroy();
    }
    
    mpi:: ~mpi() throw()
    {
        on_finalize();
    }
    
    
    int mpi:: CommWorldNext() const throw()
    {
        assert( CommWorldSize > 0 );
        return CommWorldRank >= CommWorldLast ? 0 : CommWorldRank+1;
    }
    
    int mpi:: CommWorldPrev() const throw()
    {
        assert( CommWorldSize > 0 );
        return CommWorldRank > 0 ? CommWorldRank-1 : CommWorldLast;
    }
    
    const char mpi::name[] = "MPI";
    
    int mpi:: Comm_rank( MPI_Comm comm ) const
    {
        int       r   = -1;
        const int err = MPI_Comm_rank(comm, &r);
        if( err != MPI_SUCCESS )
            throw mpi::exception( err, "MPI_Comm_rank");
        return r;
    }
    
    double mpi::Wtime() const throw()
    {
        return MPI_Wtime();
    }
    
    void  mpi:: WaitFor( double nsec) const throw()
    {
        const double stamp = Wtime();
        while( Wtime() - stamp < nsec )
            ;
    }
    
}
