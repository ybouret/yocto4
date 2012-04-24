#include "yocto/mpi/mpi.hpp"

#include <cstdarg>

namespace yocto
{
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
	
	void mpi:: Printf0( FILE *fp, const char *fmt, ... ) const
	{
		Barrier( MPI_COMM_WORLD );
		if( 0 == CommWorldRank )
		{
			va_list ap;
			va_start(ap,fmt);
			vfprintf(fp, fmt, ap);
			va_end(ap);
			fflush(fp);
		}
		Barrier(MPI_COMM_WORLD);
        
	}
    
    
    void mpi:: InitSync() const
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
    
	void mpi:: QuitSync() const
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
    
	
}

