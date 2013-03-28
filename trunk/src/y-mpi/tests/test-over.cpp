#include "yocto/utest/run.hpp"
#include "yocto/mpi/mpi.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

namespace {
    
    typedef memory::buffer_of<uint8_t, memory::global> memblock;
    
#define NUM_ITER 32
    
    static void perform_over( const mpi &MPI, size_t block_size, double &bw, double &t_ave)
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
        MPI.Printf0(stderr, "[[Synchronous Exchanges]]\n");
        unsigned long bytes = 0;
        MPI.Barrier(MPI_COMM_WORLD);
        double  t_ini = MPI.Wtime();
        for( size_t i=0; i < NUM_ITER; ++i )
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
        
        double        t_end = MPI.Wtime() - t_ini;
        double        t_sum = 0;
        unsigned long b_sum = 0;
        MPI.Reduce(&t_end, &t_sum, 1, MPI_DOUBLE,        MPI_SUM, 0, MPI_COMM_WORLD);
        MPI.Reduce(&bytes, &b_sum, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
        //MPI.Printf0(stderr,"sum_bytes=%lu/sum_time=%g\n", b_sum, t_sum);
        t_ave = t_sum/size;
        bw    = rank == 0 ? (GbitsFactor*b_sum)/(t_ave) : 0;
        MPI.Printf0(stderr, "<Gbits/s> = %.3f\n", bw);
        
        
        MPI.Printf0(stderr, "[[Asynchronous Exchanges]]\n");
        bytes = 0;
        t_ini = MPI.Wtime();
        for( size_t i=0; i <NUM_ITER; ++i )
        {
            MPI_Request req[4];
            MPI_Status  sta[4];
            size_t      ir = 0;
            MPI.Isend( send_next.ro(), block_size, MPI_BYTE, next, tag, MPI_COMM_WORLD, req[ir++]);
            MPI.Irecv( recv_next.rw(), block_size, MPI_BYTE, next, tag, MPI_COMM_WORLD, req[ir++]);
            MPI.Isend( send_prev.ro(), block_size, MPI_BYTE, prev, tag, MPI_COMM_WORLD, req[ir++]);
            MPI.Irecv( recv_prev.rw(), block_size, MPI_BYTE, prev, tag, MPI_COMM_WORLD, req[ir++]);
            
            MPI.Waitall(ir, req, sta);
        }
        t_end = MPI.Wtime() - t_ini;
        t_sum = 0;
        MPI.Reduce(&t_end, &t_sum, 1, MPI_DOUBLE,        MPI_SUM, 0, MPI_COMM_WORLD);
        MPI.Printf0(stderr, "t_ave=%g / %g\n", t_sum/size, t_ave);
        
    }
    
}

YOCTO_UNIT_TEST_IMPL(over)
{
	
    YOCTO_MPI;
    MPI.CloseStdIO();
    if( MPI.IsParallel )
    {
        FILE *fp = 0;
        if( MPI.IsFirst ) fp = fopen( "over-perf.dat", "wb");
        for( int kb=1; kb <= 1024; kb <<= 1)
        {
            double t_ave = 0;
            double bw    = 0;
            perform_over(MPI, kb*1024, bw, t_ave);
            if(MPI.IsFirst)
            {
                fprintf(fp,"%d %g %g\n", kb, bw, t_ave);
            }
        }
        if(fp) fclose(fp);
    }
    
}

YOCTO_UNIT_TEST_DONE()
