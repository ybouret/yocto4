#include "yocto/mpa/mpi.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

int main(int argc, char *argv[] )
{
    alea_init();
    try
    {
        YOCTO_MPI_ENV();
        //MPI.CloseStdIO();
        
        mpn n = MPI.CommWorldRank;
        mpz z( int64_t( 1000 * alea<double>() ));
        if(alea<double>()<0.5)
        {
            z.neg();
        }
        
        mpq q(z, uint64_t( alea<double>() * 1000 ) );
        
        
        
        MPI.Printf(stderr, "n= %u\n", n.to<unsigned>() );
        
        if(MPI.IsParallel && MPI.IsFirst)
        {
            for(int r=1;r<MPI.CommWorldSize;++r)
            {
                mpn rn = mpa::mpi_io::recv_n(MPI,r, MPI_COMM_WORLD);
                mpz rz = mpa::mpi_io::recv_z(MPI,r, MPI_COMM_WORLD);
                mpq rq = mpa::mpi_io::recv_q(MPI,r, MPI_COMM_WORLD);
                std::cerr << "received from " << r << " : " << rn << ", " << rz << ", " << rq << std::endl;
            }
        }
        else
        {
            mpa::mpi_io::send(MPI, n, 0, MPI_COMM_WORLD);
            mpa::mpi_io::send(MPI, z, 0, MPI_COMM_WORLD);
            mpa::mpi_io::send(MPI, q, 0, MPI_COMM_WORLD);
        }
        
        return 0;
    }
    catch(...)
    {
        std::cerr << "Something Bad Happened!" << std::endl;
    }
    return 1;
    
}
