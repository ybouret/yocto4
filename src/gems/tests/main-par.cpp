#include "yocto/gems/mpi.hpp"
#include "yocto/program.hpp"
#include "yocto/core/mpi-split.hpp"

using namespace yocto;
using namespace gems;

typedef atom<double> Atom;
typedef atoms<double> Atoms;
typedef binary_atoms<double> BinaryAtoms;

YOCTO_PROGRAM_START()
{
    YOCTO_MPI_ENV();
    MPI.CloseStdIO();

    MPI.Printf(stderr, "Ready...\n" );

    // common library
    library lib;
    lib.insert("H", 1.0);
    lib.insert("He",2.0);

    // atom for this node
    Atoms aa;
    if(MPI.IsFirst)
    {

        for(size_t i=1;i<=100;)
        {
            aa.insert( lib.create<double>(++i,"H") );
            aa.insert( lib.create<double>(++i,"He") );
        }
    }

    MPI.Printf(stderr, "#atoms=%u\n", unsigned(aa.size()));

    BinaryAtoms  data;
    if(MPI.IsFirst)
    {
        // get all the atoms
        const size_t Ntot = aa.size();
        for(int rank=1;rank<MPI.CommWorldSize;++rank)
        {
            // find which atoms to send to a given rank
            size_t offset = 1;
            size_t length = Ntot;
            core::mpi_split(rank, MPI.CommWorldSize, offset, length);
            MPI.Printf0(stderr, "Should send @%u(+%u) to rank=%d\n", unsigned(offset), unsigned(length), rank);

            {
                Atoms dispatch;
                for(size_t i=0;i<length;++i)
                {
                    aa.transfer_to(dispatch, offset++);
                }
                dispatch.encode(data);
                MPI.Printf0(stderr, "Ready To send %u bytes to rank %d\n\n", unsigned(data.length()),rank);
                mpi_io::send(MPI,data,rank);
            }
            lib.GC();
        }
    }
    else
    {
        mpi_io::recv(MPI,data,0);
        aa.decode(data, lib);
    }

    MPI.Printf(stderr,"Last Data Size:%u\n", unsigned(data.size));
    MPI.Printf(stderr, "#atoms=%u\n", unsigned(aa.size()));
    
    
}
YOCTO_PROGRAM_END()