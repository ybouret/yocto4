#define _CRT_SECURE_NO_WARNINGS
#include "yocto/mpi/mpi.hpp"

#include "yocto/memory/buffers.hpp"
#include "yocto/exceptions.hpp"

#include <cstdarg>
#include <cerrno>

namespace yocto
{
    void mpi:: CloseStdIO() const
    {
        if( CommWorldRank > 0 )
        {
            fclose( stdin );
            fclose( stderr );
            fclose( stdout );
        }
    }

    
    void mpi:: Printf( FILE *fp, const char *fmt, ... ) const
	{
		YOCTO_LOCK(access);
		static const int tag = 1000;
		
		assert(fmt!=NULL);
		
        //----------------------------------------------------------------------
        // start writing on rank 0
        //----------------------------------------------------------------------
        if( IsFirst )
        {
            assert(fp!=NULL);
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
		if( IsFirst )
		{
            assert(fp!=NULL);
			va_list ap;
			va_start(ap,fmt);
			vfprintf(fp, fmt, ap);
			va_end(ap);
			fflush(fp);
		}
	}
    
    void mpi:: Newline( FILE *fp ) const
    {
        if( IsFirst )
        {
            assert(fp!=NULL);
            fprintf(fp,"\n");
            fflush(fp);
        }
    }
    
    
	
}

