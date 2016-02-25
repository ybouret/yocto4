#include "yocto/mpl/mpi.hpp"
#include "yocto/program.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace mpl;

YOCTO_PROGRAM_START()
{
    YOCTO_MPI_ENV();
    MPI.CloseStdIO();
    MPI.Printf(stderr, "'%s' is ready\n",MPI.ProcessorName);
    MPI.Printf0(stderr, "ThreadLevel= %s\n", MPI.ThreadLevelName());
    
    alea_init();
    
    if(MPI.IsParallel)
    {
        {
            mpn x = 0;
            for(int iter=1;iter<=16;++iter)
            {
                MPI.Printf0(stderr, "Dispatching NATURAL, iter=%d\n",iter);
                if( MPI.IsFirst )
                {
                    // send data
                    x = mpn::rand( 2 + alea_leq(100) );
                    for(int r=1;r<MPI.CommWorldSize;++r)
                    {
                        mpl::send<mpn>(MPI,x,r,MPI_COMM_WORLD);
                    }
                }
                else
                {
                    x = mpl::recv<mpn>(MPI,0,MPI_COMM_WORLD);
                }
                
                if(MPI.IsFirst)
                {
                    for(int r=1;r<MPI.CommWorldSize;++r)
                    {
                        mpn y = mpl::recv<mpn>(MPI,r,MPI_COMM_WORLD);
                        if(x!=y)
                        {
                            MPI.Printf0(stderr, "Failure on %d\n", r);
                        }
                    }
                }
                else
                {
                    mpl::send<mpn>(MPI,x,0,MPI_COMM_WORLD);
                }
            }
        }
        
        {
            mpz z;
            for(int iter=1;iter<=16;++iter)
            {
                MPI.Printf0(stderr, "Dispatching INTEGER, iter=%d\n",iter);
                if( MPI.IsFirst )
                {
                    // send data
                    z = _rand.full<integer_t>();
                    for(int r=1;r<MPI.CommWorldSize;++r)
                    {
                        mpl::send<mpz>(MPI,z,r,MPI_COMM_WORLD);
                    }
                }
                else
                {
                    z = mpl::recv<mpz>(MPI,0,MPI_COMM_WORLD);
                }
                
                
                if(MPI.IsFirst)
                {
                    for(int r=1;r<MPI.CommWorldSize;++r)
                    {
                        mpz y = mpl::recv<mpz>(MPI,r,MPI_COMM_WORLD);
                        if(z!=y)
                        {
                            MPI.Printf0(stderr, "Failure on %d\n", r);
                        }
                    }
                }
                else
                {
                    mpl::send(MPI,z,0,MPI_COMM_WORLD);
                }
            }
            
        }
        
        {
            mpq q;
            for(int iter=1;iter<=16;++iter)
            {
                MPI.Printf0(stderr, "Dispatching Rational, iter=%d\n",iter);
                if( MPI.IsFirst )
                {
                    // send data
                    q = rational(_rand.full<integer_t>(),1+_rand.full<int32_t>());
                    for(int r=1;r<MPI.CommWorldSize;++r)
                    {
                        mpl::send<mpq>(MPI,q,r,MPI_COMM_WORLD);
                    }
                }
                else
                {
                    q = mpl::recv<mpq>(MPI,0,MPI_COMM_WORLD);
                }
                
                
                if(MPI.IsFirst)
                {
                    for(int r=1;r<MPI.CommWorldSize;++r)
                    {
                        mpq y = mpl::recv<mpq>(MPI,r,MPI_COMM_WORLD);
                        if(q!=y)
                        {
                            MPI.Printf0(stderr, "Failure on %d\n", r);
                        }
                    }
                }
                else
                {
                    mpl::send(MPI,q,0,MPI_COMM_WORLD);
                }
            }
            
        }
        
        
    }
    else
    {
        MPI.Printf0(stderr, "Not in parallel...\n");
    }
    
}
YOCTO_PROGRAM_END()
