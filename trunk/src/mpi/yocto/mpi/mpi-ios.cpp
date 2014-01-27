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
        }
    }
    
    
    ios::ostream & mpi::ostream:: operator*()
    {
        assert(MPI.IsFirst);
        assert(fp);
        return *fp;
    }
    
    
    static inline
    void __put_io_key( const mpi::io_key_t k, void *data )
    {
        assert(data);
        const mpi::io_key_t k_net = swap_be_as(k);
        memcpy(data,&k_net,sizeof(mpi::io_key_t));
    }
    
    static inline
    mpi::io_key_t __get_io_key( const void *data )
    {
        assert(data);
        mpi::io_key_t k_net(0);
        memcpy( &k_net, data, sizeof(mpi::io_key_t));
        return swap_be_as(k_net);
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
                const io_key_t k = __get_io_key(iobuff);
                if(k!=key)
                    throw yocto::exception("MPI I/O: invalid key during write char");
                fp->write(iobuff[sizeof(io_key_t)]);
            }
        }
        else
        {
            assert(!MPI.IsFirst);
            __put_io_key(key,iobuff);
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
