#include "yocto/mpl/mpi.hpp"
#include "yocto/program.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_PROGRAM_START()
{
    YOCTO_MPI_ENV();
    MPI.CloseStdIO();
    MPI.Printf(stderr, "'%s' is ready\n",MPI.ProcessorName);
    MPI.Printf0(stderr, "ThreadLevel= %s\n", MPI.ThreadLevelName());

    alea_init();

    if(MPI.IsParallel)
    {
        mpn x = 0;

        MPI.Printf0(stderr, "Dispatching a number\n");
        if( MPI.IsFirst )
        {
            // send data
            x = mpn::rand( 2 + alea_leq(100) );
            for(int r=1;r<MPI.CommWorldSize;++r)
            {
                mpl::send(MPI,x,r,MPI_COMM_WORLD);
            }
        }
        else
        {
            x = mpl::recv(MPI,0,MPI_COMM_WORLD);
        }

        MPI.Printf0(stderr, "Checking...\n");
        if(MPI.IsFirst)
        {
            for(int r=1;r<MPI.CommWorldSize;++r)
            {
                mpn y = mpl::recv(MPI,r,MPI_COMM_WORLD);
                if(x!=y)
                {
                    MPI.Printf0(stderr, "Failure on %d\n", r);
                }
            }
        }
        else
        {
            mpl::send(MPI,x,0,MPI_COMM_WORLD);
        }

        MPI.Printf0(stderr, "OK!\n");
    }
    else
    {
        MPI.Printf0(stderr, "Not in parallel...\n");
    }

}
YOCTO_PROGRAM_END()
