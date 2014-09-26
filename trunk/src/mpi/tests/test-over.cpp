#define _CRT_SECURE_NO_WARNINGS
#include "yocto/utest/run.hpp"
#include "yocto/mpi/mpi.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/threading/threads.hpp"

using namespace yocto;

namespace {
    
    typedef memory::buffer_of<uint8_t, memory::global> memblock;
    
#define NUM_ITER 128
    
    struct perf_t
    {
        double bw;
        double tmx;
    };
    
    void mpi_wait_all( mpi::Requests &requests )
    {
        static const mpi &MPI = *mpi::location();
        MPI.Waitall(requests);
    }
    
    static void perform_over(const mpi &MPI,
                             size_t     block_size,
                             perf_t    &perf_sync,
                             perf_t    &perf_async)
    {
        MPI.Printf0(stderr, "\n******** Testing %12u ********\n", unsigned(block_size) );
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
        MPI.Printf0(stderr, "\t\t[[Synchronous  Exchanges]]\n");
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
        perf_sync.tmx = t_sum/size;
        perf_sync.bw  = rank == 0 ? (GbitsFactor*b_sum)/(perf_sync.tmx) : 0;
        MPI.Printf0(stderr, "<Gbits/s> = %.3f\n", perf_sync.bw);
        
        
        MPI.Printf0(stderr, "\t\t[[Asynchronous Exchanges]]\n");
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
        perf_async.tmx = t_sum/size;
        perf_async.bw  = rank == 0 ? (GbitsFactor*b_sum)/(perf_async.tmx) : 0;
        MPI.Printf0(stderr, "<Gbits/s> = %.3f\n", perf_async.bw);
        const double t_err       = rank == 0 ? 100.0 * (perf_async.bw-perf_sync.bw)/perf_sync.bw : 0;
        MPI.Printf0(stderr, "difference: %7.2f%%\n", t_err);
        
        
        MPI.Printf0(stderr, "\t\t[[Testing Overlap]]\n");
        const double t_one = max_of( perf_sync.tmx, perf_async.tmx) / NUM_ITER;
        const double nsec  = 0.2;
        t_sum = 0;
        size_t t_count = 0;
        
        mpi::Requests requests(4);
        threading::threads thr("async");
        
        for( size_t i=0; i <16; ++i )
        {
            size_t      ir = 0;
            MPI.Irecv( recv_next.rw(), block_size, MPI_BYTE, next, tag, MPI_COMM_WORLD, requests[ir++]);
            MPI.Irecv( recv_prev.rw(), block_size, MPI_BYTE, prev, tag, MPI_COMM_WORLD, requests[ir++]);
            MPI.Isend( send_next.ro(), block_size, MPI_BYTE, next, tag, MPI_COMM_WORLD, requests[ir++]);
            MPI.Isend( send_prev.ro(), block_size, MPI_BYTE, prev, tag, MPI_COMM_WORLD, requests[ir++]);
            
            thr.start(mpi_wait_all,requests);
            
            MPI.WaitFor(nsec);
            
            t_ini = MPI.Wtime();
            thr.finish();
            t_end = MPI.Wtime() - t_ini;
            t_sum += t_end;
            ++t_count;
        }
        t_end  = t_sum/t_count;
        MPI.Printf0( stderr, "waited for %.6g/%.6g => ratio= %.6g\n", t_end,t_one, t_end/t_one);
    }
    
}

YOCTO_UNIT_TEST_IMPL(over)
{
	
    YOCTO_MPI(MPI_THREAD_MULTIPLE);
    MPI.CloseStdIO();
    if( MPI.IsParallel )
    {
        FILE *fp = 0;
        if( MPI.IsFirst ) fp = fopen( "over-perf.dat", "wb");
        for( int kb=1; kb <= 2048; kb <<= 1)
        {
            
            perf_t perf_sync  = { 0, 0 };
            perf_t perf_async = { 0, 0 };
            
            perform_over(MPI, kb*1024, perf_sync, perf_async);
            if(MPI.IsFirst)
            {
                fprintf(fp,"%d %g %g %g %g\n", kb, perf_sync.bw, perf_sync.tmx, perf_async.bw, perf_async.tmx);
            }
        }
        if(fp) fclose(fp);
    }
    
}

YOCTO_UNIT_TEST_DONE()
