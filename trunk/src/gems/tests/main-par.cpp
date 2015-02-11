#include "yocto/gems/mpi.hpp"
#include "yocto/program.hpp"

using namespace yocto;
using namespace gems;

typedef atom<double> Atom;
typedef atoms<double> Atoms;

YOCTO_PROGRAM_START()
{
    YOCTO_MPI_ENV();
    MPI.CloseStdIO();

    MPI.Printf(stderr, "Ready...\n" );

    library lib;
    lib.insert("H", 1.0);
    lib.insert("He",2.0);

    Atoms aa;
    if(MPI.IsFirst)
    {
        for(size_t i=1;i<=10;)
        {
            aa.insert( lib.create<double>(++i,"H") );
            aa.insert( lib.create<double>(++i,"He") );
        }
    }

    MPI.Printf(stderr, "#atoms=%u\n", unsigned(aa.size()));


}
YOCTO_PROGRAM_END()