#include "yocto/utest/run.hpp"

#include "yocto/mpi/comm-thread.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/hashing/sha1.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(comm)
{
	
	_rand.wseed();
    hashing::sha1 H;
    
    YOCTO_MPI;
    MPI.CloseStdIO();
    
    mpi::Requests   requests( 2 );
    mpi_comm_thread comm(MPI);
    //auto_clean<mpi_comm_thread> killComm( comm, & mpi_comm_thread::stop );
    
    const size_t block_size = 1024;
    memory::buffer_of<uint8_t, memory::global> send_next( block_size );
    memory::buffer_of<uint8_t, memory::global> recv_prev( block_size );
    
    
    for( size_t iter=0; iter <10; ++iter)
    {
        size_t ir = 0;
        MPI.Isend( send_next.ro(), block_size, MPI_BYTE, MPI.CommWorldNext(), 0, MPI_COMM_WORLD, requests[ir++]);
        MPI.Irecv( recv_prev.rw(), block_size, MPI_BYTE, MPI.CommWorldPrev(), 0, MPI_COMM_WORLD, requests[ir++]);
        
        comm.start(&requests);
    }
    
    comm.stop();
    
    //MPI.Barrier(MPI_COMM_WORLD);
    
}
YOCTO_UNIT_TEST_DONE()
