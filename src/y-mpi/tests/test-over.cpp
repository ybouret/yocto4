#include "yocto/utest/run.hpp"
#include "yocto/mpi/mpi.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

namespace {
    
    typedef memory::buffer_of<uint8_t, memory::global> memblock;
    
    static void perform_over( const mpi &MPI, size_t block_size )
    {
        MPI.Printf0(stderr, "\nTesting %u\n", unsigned(block_size) );
        const int tag = 1234;
        const double GbitsFactor = 8.0 / ( 1 << 30 );
        memblock send_next( block_size ); assert( send_next.length() == block_size );
        memblock send_prev( block_size ); assert( send_prev.length() == block_size );
        memblock recv_next( block_size ); assert( recv_next.length() == block_size );
        memblock recv_prev( block_size ); assert( recv_prev.length() == block_size );
        
        const int rank = MPI.CommWorldRank;
        const int next = MPI.CommWorldNext();
        const int prev = MPI.CommWorldPrev();
        const int size = MPI.CommWorldSize;
        
        
        _rand.wseed();
        for( size_t i=0; i < block_size; ++i )
        {
            send_next[i] = _rand.full<uint8_t>();
            send_prev[i] = _rand.full<uint8_t>();
        }
        
        //----------------------------------------------------------------------
        // statistics on sync comms
        //----------------------------------------------------------------------
        unsigned long bytes = 0;
        MPI.Barrier(MPI_COMM_WORLD);
        const double  t_ini = MPI.Wtime();
        for( size_t i=0; i < 16; ++i )
        {
            MPI_Status status;
            
            
            MPI.Sendrecv(send_next.ro(), block_size, MPI_BYTE, next, tag,
                         recv_prev.rw(), block_size, MPI_BYTE, prev, tag,
                         MPI_COMM_WORLD, status);
            bytes += 2 * block_size;
            
            MPI.Sendrecv(send_prev.ro(), block_size, MPI_BYTE, prev, tag,
                         recv_next.rw(), block_size, MPI_BYTE, next, tag,
                         MPI_COMM_WORLD, status);
            bytes += 2 * block_size;
            
            
            
        }
        
        const double t_end = MPI.Wtime() - t_ini;
        double        t_sum = 0;
        unsigned long b_sum = 0;
        MPI.Reduce(&t_end, &t_sum, 1, MPI_DOUBLE,        MPI_SUM, 0, MPI_COMM_WORLD);
        MPI.Reduce(&bytes, &b_sum, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
        //MPI.Printf0(stderr,"sum_bytes=%lu/sum_time=%g\n", b_sum, t_sum);
        const double t_ave = t_sum/size;
        const double bw = rank == 0 ? (GbitsFactor*b_sum)/(t_ave) : 0;
        MPI.Printf0(stderr, "<Gbits/s> = %.3f\n", bw);
    }
    
}

YOCTO_UNIT_TEST_IMPL(over)
{
	
    YOCTO_MPI;
    MPI.CloseStdIO();
    if( MPI.IsParallel )
    {
        for( int kb=1; kb <= 128; kb <<= 1)
        {
            perform_over(MPI, kb*1024);
        }
    }
    
}

YOCTO_UNIT_TEST_DONE()
