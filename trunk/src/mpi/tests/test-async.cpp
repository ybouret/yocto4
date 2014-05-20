#include "yocto/utest/run.hpp"

#include "yocto/mpi/async.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/hashing/sha1.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(async)
{
	
	_rand.wseed();
    hashing::sha1 H;
    
    YOCTO_MPI(MULTIPLE);
    MPI.CloseStdIO();
    
    mpi::Requests   requests( 2 );
    mpi_async       comm(MPI);
    
    const size_t block_size = 16;
    memory::buffer_of<uint8_t, memory::global> send_next( block_size );
    memory::buffer_of<uint8_t, memory::global> recv_prev( block_size );
    
    for( unsigned iter=1; iter <= 1024; ++iter)
    {
        size_t ir = 0;
        MPI.Irecv( recv_prev.rw(), block_size, MPI_BYTE, MPI.CommWorldPrev(), 0, MPI_COMM_WORLD, requests[ir++]);
        MPI.Isend( send_next.ro(), block_size, MPI_BYTE, MPI.CommWorldNext(), 0, MPI_COMM_WORLD, requests[ir++]);
        
        comm.launch(requests);
        comm.finish();
        
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
