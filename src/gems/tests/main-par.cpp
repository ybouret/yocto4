#include "yocto/gems/mpi.hpp"
#include "yocto/program.hpp"
#include "yocto/core/mpi-split.hpp"
#include "yocto/gems/io/crystal.hpp"
#include "yocto/ios/ocstream.hpp"

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
    lib.insert("Na", 23);
    lib.insert("Cl", 35);

    coord_t      cr;
    v3d<double>  box(12,13,14);
    const size_t n = crystal::close_packed_count(cr, 100, box);

    // atom for this node
    Atoms aa;
    if(MPI.IsFirst)
    {


        for(size_t i=1;i<=n;++i)
        {
            if(i&1)
            {
                aa.insert( lib.create<double>(i,"Na") );
            }
            else
            {
                aa.insert( lib.create<double>(i,"Cl"));
            }
        }

        crystal::build_fcc(aa,cr,box);
        ios::ocstream fp("fcc.xyz",false);
        aa.save_xyz(fp, "full fcc");
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

    MPI.Printf(stderr, "#atoms=%u\n", unsigned(aa.size()));
    {
        ios::ocstream fp( vformat("fcc%d.%d.xyz", MPI.CommWorldSize, MPI.CommWorldRank),false);
        aa.save_xyz(fp, "partial fcc");
    }
    
}
YOCTO_PROGRAM_END()