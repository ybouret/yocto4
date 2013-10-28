#include "yocto/mpi/mpi.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/memory/buffers.hpp"

namespace yocto
{
    
    mpi::ostream:: ~ostream() throw()
    {
        if(fp)
        {
            assert(MPI.IsFirst);
            delete fp;
            fp = 0;
        }
    }
    
    mpi::ostream:: ostream( const mpi &ref, const string &filename, bool append ) :
    ios::ostream(),
    MPI(ref),
    key( MPI.hasher.key<io_key_t>(filename)),
    fp(0)
    {
        if( MPI.IsFirst )
        {
            fp = new ios::ocstream(filename,append);
        }
    }
    
    
    ios::ostream & mpi::ostream:: operator*()
    {
        assert(MPI.IsFirst);
        assert(fp);
        return *fp;
    }
    
    void mpi::ostream:: write(char C)
    {
        char iobuff[sizeof(io_key_t)+1];
        if(fp)
        {
            assert(MPI.IsFirst);
            // print my char
            fp->write(C);
            MPI_Status status;
            for(int r=1;r<MPI.CommWorldSize;++r)
            {
                MPI.Recv(iobuff, sizeof(iobuff), MPI_BYTE, r, tag, MPI_COMM_WORLD, status);
                const io_key_t k = swap_be_as<io_key_t>( *(io_key_t *)iobuff );
                if(k!=key)
                    throw yocto::exception("MPI I/O: invalid key");
                fp->write(iobuff[sizeof(io_key_t)]);
            }
        }
        else
        {
            assert(!MPI.IsFirst);
            *(io_key_t *)iobuff = swap_be_as<io_key_t>(key);
            iobuff[sizeof(io_key_t)]=C;
            MPI.Send(iobuff, sizeof(iobuff), MPI_BYTE, 0, tag, MPI_COMM_WORLD);
        }
    }
    
    void mpi:: ostream:: flush()
    {
        if(fp)
            fp->flush();
    }
    
}
