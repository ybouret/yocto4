#include "yocto/mpi/mpi.hpp"

#include "yocto/memory/buffers.hpp"
#include "yocto/exceptions.hpp"

#include <cstdarg>
#include <cerrno>

namespace yocto
{
    
#if 0
	void mpi:: Printf( FILE *fp, const char *fmt, ... ) const
	{
		
		static const int tag = 1000;
		int              msg = tag;
		
		assert(fp!=NULL);
		assert(fmt!=NULL);
		
		
		
		if( CommWorldSize > 1 )
		{
			Barrier( MPI_COMM_WORLD );
			
			if( CommWorldRank > 0 )
			{
				MPI_Status status;
				Recv( &msg, 1, MPI_INT, CommWorldRank-1, tag, MPI_COMM_WORLD,status);
			}
			fflush( fp );
			va_list ap;
			va_start(ap,fmt);
			vfprintf(fp, fmt, ap);
			va_end(ap);
			fflush(fp);
            
			if( CommWorldRank < CommWorldLast )
			{
				Send( &msg, 1, MPI_INT, CommWorldRank+1, tag, MPI_COMM_WORLD);
			}
			
			
			Barrier( MPI_COMM_WORLD );
		}
		else
		{
			// only one
			fflush( fp );
			va_list ap;
			va_start(ap,fmt);
			vfprintf(fp, fmt, ap);
			va_end(ap);
			fflush(fp);
		}
		
		
	}
#endif
    
    void mpi:: Printf( FILE *fp, const char *fmt, ... ) const
	{
		
		static const int tag = 1000;
		
		assert(fp!=NULL);
		assert(fmt!=NULL);
		
        //----------------------------------------------------------------------
        // start writing on rank 0
        //----------------------------------------------------------------------
        if( IsFirst )
        {
            va_list ap;
            va_start(ap,fmt);
            fprintf(fp,"%d.%d> ", CommWorldSize, CommWorldRank);
            vfprintf(fp, fmt, ap);
            va_end(ap);
            fflush(fp);
        }
        
        if( IsParallel )
        {
            if( IsFirst )
            {
                //-- wait sub messages, in rank order
                for( int i=1; i < CommWorldSize; ++i )
                {
                    MPI_Status status;
                    const string msg = Recv(i, tag, MPI_COMM_WORLD, status);
                    fprintf(fp,"%d.%d> ", CommWorldSize, i);
                    fprintf(stderr, "%s", msg.c_str());
                }
            }
            else
            {
                //-- send sub messages
                size_t n = 32;
            TRY:
                {
                    memory::buffer_of<char,memory::global> databuf( n );
                    char  *buffer  = databuf();
                    int    length  = int( databuf.length() );
                    if( length < 0 )
                        throw libc::exception( ERANGE, "MPI::Printf memory overflow");
                    
                    va_list ap;
                    va_start( ap, fmt );
                    const int  result  = vsnprintf( buffer, length, fmt, ap);
                    const bool success = (result >= 0 && result < length); // WIN32 and GLIBC compatible
                    va_end (ap);
                    
                    if( success )
                    {
                        const string msg( buffer );
                        Send(msg, 0, tag, MPI_COMM_WORLD);
                        return;
                    }
                    n <<= 1;
                }
                goto TRY;
            }
        }
        
        
        
	}
    
    
	
	void mpi:: Printf0( FILE *fp, const char *fmt, ... ) const
	{
		if( 0 == CommWorldRank )
		{
			va_list ap;
			va_start(ap,fmt);
			vfprintf(fp, fmt, ap);
			va_end(ap);
			fflush(fp);
		}
	}
    
    
#if 0
    void mpi:: __InitSync() const
    {
        static const int tag = 1001;
		int              msg = tag;
        if( CommWorldSize > 1 )
        {
            Barrier(MPI_COMM_WORLD);
            if( CommWorldRank > 0 )
			{
                // wait from previous
                // fprintf( stderr, "[MPI] rank %d InitSync\n", CommWorldRank ); fflush( stderr );
				MPI_Status status;
				Recv( &msg, 1, MPI_INT, CommWorldRank-1, tag, MPI_COMM_WORLD,status);
                
                // send to next
                if( CommWorldRank < CommWorldLast )
                {
                    Send( &msg, 1, MPI_INT, CommWorldRank+1, tag, MPI_COMM_WORLD);
                }
                //fprintf( stderr, "[MPI] rank %d QuitSync\n", CommWorldRank ); fflush( stderr );
			}
        }
    }
    
	void mpi:: __QuitSync() const
    {
        static const int tag = 1001;
		int              msg = tag;
        if( CommWorldSize > 1 )
        {
            if( 0 == CommWorldRank )
            {
                Send( &msg, 1, MPI_INT, CommWorldRank+1, tag, MPI_COMM_WORLD);
            }
            Barrier(MPI_COMM_WORLD);
        }
    }
#endif
    
	
}

