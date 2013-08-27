#include "yocto/mpi/mpi.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/memory/buffers.hpp"

namespace yocto
{
    
    namespace
    {
        typedef memory::fixed_length_buffer<mpi::hashing_function::__length> mpi_iokey;
        
        class mpi_ostream : public object
        {
        public:
            static const int tag = 9;
            const mpi       &MPI;
            const string     fn;
            ios::ocstream   *fp;
            const mpi_iokey  key;
            
            explicit mpi_ostream(const mpi    &user_ref,
                                 const string &filename,
                                 bool          append ) :
            MPI(user_ref),
            fn(filename),
            fp(0),
            key()
            {
                if(MPI.IsFirst)
                {
                    fp = new ios::ocstream(fn,append);
                }
                
                MPI.hasher.set();
                MPI.hasher(filename);
                MPI.hasher.out( (mpi_iokey&)key );
            }
            
            virtual ~mpi_ostream() throw()
            {
                if(fp)
                {
                    delete fp;
                }
            }
            
            inline void write(char C)
            {
                match_io();
                if(fp)
                {
                    assert(MPI.IsFirst);
                    MPI_Status status;
                    fp->write(C);
                    for(int r=1; r < MPI.CommWorldSize; ++r)
                    {
                        MPI.Recv(&C, 1, MPI_CHAR, r, tag, MPI_COMM_WORLD, status);
                        fp->write(C);
                    }
                }
                else
                {
                    MPI.Send(&C, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
                }
            }
            
            inline void flush()
            {
                match_io();
                if(fp) { fp->flush(); }
            }
            
            inline void match_io() const
            {
                if(fp)
                {
                    assert(MPI.IsFirst);
                    mpi_iokey  chk;
                    MPI_Status status;
                    for(int r=1; r < MPI.CommWorldSize; ++r)
                    {
                        MPI.Recv(chk.rw(), chk.length(), MPI_BYTE, r, tag, MPI_COMM_WORLD, status);
                        if( key != chk )
                            throw yocto::exception("mpi::ostream(file mismatch)");
                    }
                }
                else
                {
                    assert(MPI.CommWorldRank>0);
                    MPI.Send(key.ro(), key.length(), MPI_BYTE, 0, tag, MPI_COMM_WORLD);
                }
            }
            
            inline void put(const void *data, size_t size, size_t &done)
            {
                
                done = 0;
                match_io();
                if(fp)
                {
                    assert(MPI.IsFirst);
                    
                    //-- append root
                    fp->append((const char *)data,size);
                    
                    //-- append others
                    MPI_Status status;
                    for(int r=1;r<MPI.CommWorldSize;++r)
                    {
                        const size_t in_size = MPI.Recv<size_t>(r, tag, MPI_COMM_WORLD, status);
                        if(in_size>0)
                        {
                            memory::buffer_of<char,memory::global> in_buff(in_size);
                            MPI.Recv(in_buff.rw(), in_size, MPI_BYTE, r, tag, MPI_COMM_WORLD, status);
                            fp->append(in_buff(0), in_size);
                        }
                    }
                    
                    //-- all done
                    done = size;
                }
                else
                {
                    assert(MPI.CommWorldRank>0);
                    MPI.Send<size_t>(size, 0, tag, MPI_COMM_WORLD);
                    if(size>0)
                        MPI.Send(data, size, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
                    
                    //-- all done
                    done = size;
                }
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(mpi_ostream);
        };
    }
    
    mpi::ostream:: ostream(const mpi    &MPI,
                           const string &filename,
                           bool          append) :
    impl(0)
    {
        impl = new mpi_ostream(MPI,filename,append);
    }
    
    mpi::ostream::ostream(const mpi  &MPI,
                          const char *filename,
                          bool        append) :
    impl(0)
    {
        const string fn(filename);
        impl = new mpi_ostream(MPI,fn,append);
    }
    
    mpi::ostream:: ~ostream() throw()
    {
        assert(impl);
        delete static_cast<mpi_ostream*>(impl);
        impl = 0 ;
    }
    
    void mpi:: ostream:: write(char C)
    {
        assert(impl);
        static_cast<mpi_ostream*>(impl)->write(C);
    }
    
    void mpi:: ostream:: flush()
    {
        assert(impl);
        static_cast<mpi_ostream*>(impl)->flush();
    }
    
    void mpi:: ostream:: put(const void *data, size_t size, size_t &done)
    {
        assert(impl);
        static_cast<mpi_ostream*>(impl)->put(data,size,done);
    }
    
    ios::ostream &  mpi:: ostream:: operator*()
    {
        assert(impl);
        mpi_ostream *w = static_cast<mpi_ostream*>(impl);
        if(0 == w->fp )
        {
            throw yocto::exception("mpi::ocstream(not on root)");
        }
        return *(w->fp);
    }
    
}
