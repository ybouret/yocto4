#include "yocto/utest/run.hpp"
#include "yocto/mpi/mpi.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(ios)
{
    YOCTO_MPI_ENV();
    {
        mpi::ostream fp( MPI, "foo.dat", false );
        if(MPI.IsFirst)
        {
            *fp << "Header\n";
        }
        fp.write( 'A' + MPI.CommWorldRank);
        if(MPI.IsFirst) *fp << "\n";
        const string str = vformat("On Node %d.%d\n", MPI.CommWorldSize, MPI.CommWorldRank);
        size_t done = 0;
        fp.put(str.ro(), str.size(), done);
        fp("Hello from %d.%d\n", MPI.CommWorldSize, MPI.CommWorldRank);
        
        if(MPI.IsFirst) *fp << "Epilog\n";
        //MPI.Barrier(MPI_COMM_WORLD);
        //MPI.Printf(stderr,"Done\n");
    }
    
}
YOCTO_UNIT_TEST_DONE()
