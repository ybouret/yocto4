#include "yocto/mpi/mpi.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/memory/buffers.hpp"

#include <cstring>

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
            try
            {
                for(int r=1;r<MPI.CommWorldSize;++r)
                {
                    MPI.Send(key, r, tag, MPI_COMM_WORLD);
                }
            }
            catch(...)
            {
                delete fp;
                throw;
            }
        }
        else
        {
            MPI_Status status;
            const io_key_t master_key = MPI.Recv<io_key_t>(0, tag, MPI_COMM_WORLD, status);
            if(key!=master_key)
            {
                throw yocto::exception("mpi::ostream: key mismatch!");
            }
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
        static const char iosize = sizeof(io_key_t)+1;
        char iobuff[sizeof(io_key_t)*2];

        if(fp)
        {
            assert(MPI.IsFirst);
            // print my char
            fp->write(C);
            MPI_Status status;
            for(int r=1;r<MPI.CommWorldSize;++r)
            {
                MPI.Recv(iobuff,iosize, MPI_BYTE, r, tag, MPI_COMM_WORLD, status);
                const io_key_t k = query_be<io_key_t>(iobuff);
                if(k!=key)
                    throw yocto::exception("MPI I/O: invalid key during write char");
                fp->write(iobuff[sizeof(io_key_t)]);
            }
        }
        else
        {
            assert(!MPI.IsFirst);
            store_be<io_key_t>(key,iobuff);
            //*(io_key_t *)iobuff = swap_be_as<io_key_t>(key);
            iobuff[sizeof(io_key_t)]=C;
            MPI.Send(iobuff, iosize, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
        }
    }

    void mpi:: ostream:: flush()
    {
        if(fp)


            fp->flush();
    }

    void mpi::ostream:: put( const void *buff, size_t size, size_t &done)
    {
        assert(!(0==buff&&size>0));
        done=0;
        if(fp)
        {
            assert(MPI.IsFirst);
            //-- master writes its buffer
            fp->save(buff, size);
            MPI_Status status;

            // then append other buffers
            for(int r=1; r < MPI.CommWorldSize; ++r)
            {
                //-- recv authentication
                if( MPI.Recv<io_key_t>(r, tag, MPI_COMM_WORLD, status) != key )
                    throw yocto::exception("MPI I/O: invalid key during put buffer");

                //-- recv length
                const size_t rsize = MPI.Recv<size_t>(r,tag,MPI_COMM_WORLD,status);

                //-- recv buffer
                memory::buffer_of<char, memory::global> rbuff( rsize ); assert( rbuff.length() == rsize);
                MPI.Recv( rbuff.rw(), rsize, MPI_BYTE, r, tag, MPI_COMM_WORLD, status);

                //-- write it !
                *fp << rbuff;
            }

            // all done
            done = size;
        }
        else
        {
            //-- send authentication
            MPI.Send<io_key_t>(key,0,tag,MPI_COMM_WORLD);

            //-- send buffer length
            MPI.Send<size_t>(size,0,tag,MPI_COMM_WORLD);

            //-- send buffer
            MPI.Send(buff, size, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
            done = size;
        }
    }

}

#include "yocto/ios/icstream.hpp"
namespace yocto
{

    mpi::istream:: ~istream() throw()
    {
        if(fp)
        {
            assert(MPI.IsFirst);
            delete fp;
            fp = 0;
        }
    }

    mpi:: istream:: istream(const mpi &ref, const string &filename ) :
    ios::istream(),
    MPI(ref),
    key( MPI.hasher.key<io_key_t>(filename)),
    fp(0)
    {
        if( MPI.IsFirst )
        {
            fp = new ios::icstream(filename);
            try
            {
                for(int r=1;r<MPI.CommWorldSize;++r)
                {
                    MPI.Send(key, r, tag, MPI_COMM_WORLD);
                }
            }
            catch(...)
            {
                delete fp;
                throw;
            }
        }
        else
        {
            MPI_Status     status;
            const io_key_t master_key = MPI.Recv<io_key_t>(0, tag, MPI_COMM_WORLD, status);
            if(key!=master_key)
            {
                throw yocto::exception("mpi::istream: key mismatch!");
            }
        }

        
    }


    bool mpi::istream:: query( char &C )
    {
        char CC[2] = {0,0};
        if(MPI.IsFirst)
        {
            if( fp->query(C) )
            {
                CC[0]=1;
                CC[1]=C;
            }
            for(int r=1;r<MPI.CommWorldSize;++r)
            {
                MPI.Send(CC, 2, MPI_CHAR, r, tag, MPI_COMM_WORLD);
            }
        }
        else
        {
            MPI_Status status;
            MPI.Recv(CC, 2, MPI_CHAR, 0, tag, MPI_COMM_WORLD, status);
            C=CC[1];
        }
        return 1 == CC[0];
    }

    void mpi::istream:: store(char C)
    {
        if(MPI.IsFirst)
        {
            fp->store(C);
        }
    }

    void mpi::istream:: get(void *data, size_t size, size_t &done)
    {
        if(MPI.IsFirst)
        {
            fp->get(data, size, done);
            const uint32_t done32 = done;
            for(int r=1;r<MPI.CommWorldSize;++r)
            {
                MPI.Send(done32, r, tag, MPI_COMM_WORLD);
                if(done32)
                {
                    MPI.Send(data, size, MPI_BYTE, r, tag, MPI_COMM_WORLD);
                }
            }
        }
        else
        {
            MPI_Status status;
            done = MPI.Recv<uint32_t>(0, tag, MPI_COMM_WORLD, status);
            assert(done<=size);
            if(done)
            {
                MPI.Recv(data, done, MPI_BYTE, 0, tag, MPI_COMM_WORLD, status);
            }
        }
    }


}


