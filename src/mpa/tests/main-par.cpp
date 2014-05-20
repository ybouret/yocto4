#include "yocto/mpa/mpi.hpp"

using namespace yocto;

int main(int argc, char *argv[] )
{
    
    try
    {
        YOCTO_MPI;
        //MPI.CloseStdIO();
        
        mpn n = MPI.CommWorldRank;
        MPI.Printf(stderr, "n= %u\n", n.to<unsigned>() );
        if(MPI.IsParallel && MPI.IsFirst)
        {
            for(int r=1;r<MPI.CommWorldSize;++r)
            {
                mpn m = mpa::mpi_io::recv_n(MPI, r, MPI_COMM_WORLD);
                MPI.Printf0(stderr, "received from %d: %u\n", r, m.to<unsigned>() );
            }
        }
        else
        {
            mpa::mpi_io::send(MPI, n, 0, MPI_COMM_WORLD);
        }
        
        return 0;
    }
    catch(...)
    {
        std::cerr << "Something Bad Happened!" << std::endl;
    }
    return 1;
    
}
