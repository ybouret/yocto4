#include "yocto/utest/run.hpp"
#include "yocto/mpi/mpi.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(ios)
{
    YOCTO_MPI;
    {
        mpi::ostream fp( MPI, "foo.dat", false );
        if(MPI.IsFirst)
        {
            *fp << "Header\n";
        }
        fp.write( 'A' + MPI.CommWorldRank);
        if(MPI.IsFirst)
            *fp << "\n";
        fp("On Node %d.%d\n", MPI.CommWorldSize, MPI.CommWorldRank);
        //fp.flush();
        MPI.Printf(stderr,"Done\n");
    }
}
YOCTO_UNIT_TEST_DONE()
