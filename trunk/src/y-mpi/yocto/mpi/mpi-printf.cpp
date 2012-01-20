#include "yocto/mpi/mpi.hpp"

#include <cstdarg>

namespace yocto
{
	void mpi:: Printf( FILE *fp, const char *fmt, ... ) const
	{
		
		static const int tag = 1000;
		int              msg = 1000;
		
		assert(fp!=NULL);
		assert(fmt!=NULL);
		
		
		
		if( CommWorldSize > 1 )
		{
			Barrier( MPI_COMM_WORLD );
			
			if( CommWorldRank > 0 )
			{
				MPI_Status status;
				Recv( &msg, 1, MPI_INT, CommWorldPrev(), tag, MPI_COMM_WORLD,status);
			}
			
			va_list ap;
			va_start(ap,fmt);
			vfprintf(fp, fmt, ap);
			va_end(ap);

			fflush(fp);

			if( CommWorldRank < CommWorldRankMax )
			{
				Send( &msg, 1, MPI_INT, CommWorldNext(), tag, MPI_COMM_WORLD);
			}
			
			
			Barrier( MPI_COMM_WORLD );
		}
		else
		{
			// only one
			va_list ap;
			va_start(ap,fmt);
			vfprintf(fp, fmt, ap);
			va_end(ap);
		}
		
		
	}
	
}