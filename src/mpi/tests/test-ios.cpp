#include "yocto/utest/run.hpp"
#include "yocto/mpi/mpi.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(ostream)
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

YOCTO_UNIT_TEST_IMPL(istream)
{
    YOCTO_MPI_ENV();
    MPI.Barrier(MPI_COMM_WORLD);
    if(argc>1)
    {
        const string filename = argv[1];
        mpi::istream fp(MPI,filename);

        int32_t code=0;
        size_t done;
        fp.get(&code,4,done);
        MPI.Printf(stderr, "done=%u\n", unsigned(done));
        MPI.Printf(stderr, "word=%08x\n", code);
        MPI.Barrier(MPI_COMM_WORLD);

        string line;
        int    count=0;
        mpi::hashing_function H;
        H.set();
        while( line.clear(), fp.read_line(line)>0 )
        {
            MPI.Printf(stderr,"%10d: %s\n",++count,line.c_str());
            H(line);
        }
        const unsigned hk = H.key<unsigned>();
        MPI.Printf(stderr, "HK=%u\n", hk);
    }

}
YOCTO_UNIT_TEST_DONE()
